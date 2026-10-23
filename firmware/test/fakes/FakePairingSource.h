#pragma once

#include <string>

#include "Communication/PairingSource.h"

class FakePairingSource : public PairingSource {
public:
    const char* pairedChatId() const override {
        return chatId_.c_str();
    }

    const char* pairingCode() const override {
        return code_.c_str();
    }

    void bind(const char* chatId) override {
        chatId_ = chatId;
        ++bindCount;
    }

    /**
     * @brief Sets the code TelegramMessageSource should treat as correct.
     */
    void setCode(const char* code) {
        code_ = code;
    }

    /**
     * @brief Seeds the fake as if a Sender had already been claimed.
     */
    void seedPairedChatId(const char* chatId) {
        chatId_ = chatId;
    }

    int bindCount = 0;

private:
    std::string chatId_;
    std::string code_ = "123456";
};
