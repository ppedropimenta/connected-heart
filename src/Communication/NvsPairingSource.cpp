#include "NvsPairingSource.h"

#include <cstdio>

#include <Arduino.h>
#include <Preferences.h>

namespace {
    constexpr const char* Namespace = "pairing";
    constexpr const char* ChatIdKey = "chat_id";
    constexpr const char* CodeKey = "code";

    /**
     * @brief Formats a fresh six-digit pairing code.
     *
     * random() draws from the ESP32's hardware RNG, so no explicit seed is
     * needed for this to be unpredictable.
     */
    void generateCode(char* buffer, const size_t bufferSize) {
        const long value = random(0, 1000000L);
        std::snprintf(buffer, bufferSize, "%06ld", value);
    }
}

const char* NvsPairingSource::pairedChatId() const {
    Preferences preferences;

    if (preferences.begin(Namespace, true)) {
        preferences.getString(ChatIdKey, chatId_, sizeof(chatId_));
        preferences.end();
    }

    return chatId_;
}

const char* NvsPairingSource::pairingCode() const {
    Preferences preferences;

    if (preferences.begin(Namespace, true)) {
        preferences.getString(CodeKey, code_, sizeof(code_));
        preferences.end();
    }

    return code_;
}

void NvsPairingSource::ensureCode() {
    Preferences preferences;

    if (preferences.begin(Namespace, true)) {
        const bool hasCode = preferences.isKey(CodeKey);
        preferences.end();

        if (hasCode) {
            return;
        }
    }

    char code[7];
    generateCode(code, sizeof(code));

    if (preferences.begin(Namespace, false)) {
        preferences.putString(CodeKey, code);
        preferences.end();
    }
}

void NvsPairingSource::bind(const char* chatId) {
    Preferences preferences;

    if (!preferences.begin(Namespace, false)) {
        return;
    }

    preferences.putString(ChatIdKey, chatId);

    preferences.end();
}
