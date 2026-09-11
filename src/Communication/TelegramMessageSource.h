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
 * The paired sender's chat identifier comes from a PairingSource, whose
 * only implementation for now (CompiledPairingSource) reads it in
 * compiled alongside the WiFi credentials; the pairing-by-code flow that
 * learns it at runtime is ticket 12, behind the same seam. Acknowledgements
 * and rejection replies are not sent here; see ticket 10.
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
     * construction (ticket 12) takes effect without restarting the
     * heart. Must outlive this object.
     */
    TelegramMessageSource(const char* botToken, PairingSource& pairingSource);

    void begin() override;
    void update() override;

    bool hasMessage() const override;
    HeartMessage takeMessage() override;

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

    PairingSource& pairingSource_;

    WiFiClientSecure client_;
    UniversalTelegramBot bot_;

    unsigned long lastPollAt_ = 0;

    HeartMessage pending_ = HeartMessage(Feeling::Love);
    bool hasPending_ = false;
};
