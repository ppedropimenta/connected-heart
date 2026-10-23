/**
* @file TelegramMessageSource.h
 * @brief Delivers messages sent to the heart's own Telegram bot.
 *
 * Polls Telegram's getUpdates endpoint on an interval, discarding every
 * update except those from the paired sender's chat, and hands an
 * accepted update to the application through the existing MessageSource
 * interface. A selected Feeling command — the same commands
 * SerialLineParser recognizes for SerialMessageSource's bench harness —
 * becomes a Predefined Message; any other text becomes a Custom Message.
 *
 * The paired sender's chat identifier comes from a PairingSource. While
 * unpaired (PairingSource::isPaired() false), every update is instead
 * checked against PairingSource::pairingCode() through PairingCodeMatcher
 * (ADR-0004): a match claims the chat via PairingSource::bind() and is
 * acknowledged, and anything else — a wrong code, an empty message,
 * unsupported content — is silently discarded, so a stranger who finds
 * the bot before pairing learns nothing from it. CompiledPairingSource,
 * for bench work, is always already paired and never takes this path.
 *
 * Per ADR-0002, the bot also replies to the sender once
 * paired: an update with no usable text (empty, or unsupported content
 * such as a photo, sticker, or voice note) is rejected immediately in
 * handleUpdate(); an update that resolves to a HeartMessage is
 * acknowledged or rejected as too long from reportOutcome(), once
 * Application has told this source what MessageManager decided. An
 * update from an unpaired chat gets no reply of any kind. Wording lives
 * in AcknowledgementCatalog, which is unit tested on the host since this
 * class is not.
 *
 * This is a thin wrapper over UniversalTelegramBot, a third-party library
 * over Telegram's HTTP API, and is not unit tested for the same reason
 * WifiManager and EpaperDisplay are not: testing it would mean
 * simulating the library rather than the product.
 */

#pragma once

#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

#include "Communication/MessageSource.h"
#include "Communication/PairingSource.h"
#include "Message/Feeling.h"
#include "Message/HeartMessage.h"

class TelegramMessageSource final : public MessageSource {
public:
    /**
     * @param botToken This heart's own bot token, per ADR-0003. Must
     * outlive this object.
     * @param pairingSource Supplies the paired sender's chat identifier,
     * queried fresh on every update so a pairing completed after
     * construction takes effect without restarting the
     * heart. Must outlive this object.
     */
    TelegramMessageSource(const char* botToken, PairingSource& pairingSource);

    void begin() override;
    void update() override;

    bool hasMessage() const override;
    HeartMessage takeMessage() override;

    void reportOutcome(bool accepted, bool wasPendingDisplaced) override;

private:
    /**
     * Safety bound on how many sequential getUpdates() calls a single
     * poll will drain, so a backlog cannot monopolize the main loop
     * indefinitely; any remainder simply drains on later polls. Ordinary
     * use for one couple never comes close.
     */
    static constexpr int MaxDrainIterationsPerPoll = 20;

    void poll();
    void handleUpdate(const telegramMessage& message);
    void handlePairingAttempt(const telegramMessage& message);
    void reply(const String& chatId, const char* text);

    PairingSource& pairingSource_;

    WiFiClientSecure client_;
    UniversalTelegramBot bot_;

    unsigned long lastPollAt_ = 0;

    HeartMessage pending_ = HeartMessage(Feeling::Love);
    bool hasPending_ = false;

    /**
     * Chat to reply to from reportOutcome(), remembered from the update
     * that produced pending_. A single-slot buffer like pending_ itself:
     * safe because Application always drains one message (and reports
     * its outcome) before this source's next update() can produce another.
     */
    String pendingChatId_;
};
