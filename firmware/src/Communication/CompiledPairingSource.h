/**
* @file CompiledPairingSource.h
 * @brief Supplies the paired sender's chat identifier compiled into the
 * firmware, for bench work that skips pairing by code.
 */

#pragma once

#include "PairingSource.h"

/**
 * @brief Reads the paired chat identifier from Secrets.h, which is
 * excluded from version control; see Secrets.example.h for the expected
 * shape.
 */
class CompiledPairingSource : public PairingSource {
public:
    const char* pairedChatId() const override;
    const char* pairingCode() const override;
    void bind(const char* chatId) override;
};
