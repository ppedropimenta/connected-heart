/**
* @file CompiledPairingSource.h
 * @brief Supplies the paired sender's chat identifier compiled into the
 * firmware, for bench work ahead of pairing by code (ticket 12).
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
};
