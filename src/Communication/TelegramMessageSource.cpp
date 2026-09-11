#include "TelegramMessageSource.h"

#include <WiFi.h>

#include "Communication/MessageTextResolver.h"
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

    if (message.chat_id != pairingSource_.pairedChatId()) {
        LOG_DEBUG("Telegram update from unpaired chat discarded");
        return;
    }

    if (message.text.length() == 0) {
        return;
    }

    pending_ = MessageTextResolver::resolve(message.text.c_str());
    hasPending_ = true;

    LOG_INFO("Telegram message received");
}
