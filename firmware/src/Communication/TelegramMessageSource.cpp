#include "TelegramMessageSource.h"

#include <WiFi.h>

#include "Communication/AcknowledgementCatalog.h"
#include "Communication/MessageTextResolver.h"
#include "Communication/PairingCodeMatcher.h"
#include "Core/Config.h"
#include "Core/Log.h"

TelegramMessageSource::TelegramMessageSource(const char* botToken, PairingSource& pairingSource)
    : pairingSource_(pairingSource)
    , bot_(botToken, client_)
{
}

void TelegramMessageSource::begin() {
    /*
     * Skips certificate validation rather than pinning Telegram's root
     * CA. This is a hand-built gift for one couple's traffic, not a
     * service exposed to the public, and it is the same shortcut
     * UniversalTelegramBot's own ESP32 examples take.
     */
    client_.setInsecure();
}

void TelegramMessageSource::update() {
    if (WiFi.status() != WL_CONNECTED) {
        return;
    }

    if (millis() - lastPollAt_ < Config::TelegramPollIntervalMs) {
        return;
    }

    lastPollAt_ = millis();

    poll();
}

bool TelegramMessageSource::hasMessage() const {
    return hasPending_;
}

HeartMessage TelegramMessageSource::takeMessage() {
    hasPending_ = false;
    return pending_;
}

void TelegramMessageSource::poll() {
    int numNewMessages;
    int iterations = 0;

    do {
        numNewMessages = bot_.getUpdates(bot_.last_message_received + 1);

        for (int i = 0; i < numNewMessages; ++i) {
            handleUpdate(bot_.messages[i]);
        }

        ++iterations;
    } while (numNewMessages > 0 && iterations < MaxDrainIterationsPerPoll);
}

void TelegramMessageSource::handleUpdate(const telegramMessage& message) {
    if (message.type != "message") {
        return;
    }

    if (!pairingSource_.isPaired()) {
        handlePairingAttempt(message);
        return;
    }

    if (message.chat_id != pairingSource_.pairedChatId()) {
        LOG_DEBUG("Telegram update from unpaired chat discarded");
        return;
    }

    if (message.text.length() == 0) {
        reply(message.chat_id, AcknowledgementCatalog::rejectedNotText());
        LOG_INFO("Telegram update with no usable text rejected");
        return;
    }

    pending_ = MessageTextResolver::resolve(message.text.c_str());
    hasPending_ = true;
    pendingChatId_ = message.chat_id;

    LOG_INFO("Telegram message received");
}

void TelegramMessageSource::handlePairingAttempt(const telegramMessage& message) {
    if (!PairingCodeMatcher::matches(pairingSource_.pairingCode(), message.text.c_str())) {
        /*
         * Silent by design: a wrong code binds nothing and gets no
         * reply, so a stranger who finds the bot before pairing learns
         * nothing from it.
         */
        LOG_DEBUG("Telegram pairing attempt with wrong code discarded");
        return;
    }

    pairingSource_.bind(message.chat_id.c_str());

    LOG_INFO("Telegram chat paired");

    reply(message.chat_id, AcknowledgementCatalog::paired());
}

void TelegramMessageSource::reportOutcome(const bool accepted, const bool wasPendingDisplaced) {
    if (accepted) {
        reply(
            pendingChatId_,
            wasPendingDisplaced
                ? AcknowledgementCatalog::deliveredReplacingUnrevealed()
                : AcknowledgementCatalog::delivered()
        );
        return;
    }

    /*
     * The only way MessageManager can reject a message that reached this
     * point is the length limit: an empty Custom Message can't reach
     * here, since the empty-text branch above returns before pending_ is
     * ever set, and a Predefined Message is always valid.
     */
    reply(pendingChatId_, AcknowledgementCatalog::rejectedTooLong());
}

void TelegramMessageSource::reply(const String& chatId, const char* text) {
    bot_.sendMessage(chatId, text, "");
}
