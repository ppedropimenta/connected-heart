/**
* @file NvsPairingSource.h
 * @brief Supplies and claims the paired sender's chat identifier, learned
 * through pairing by code and persisted in ESP32 NVS.
 */

#pragma once

#include "PairingSource.h"

/**
 * @brief Reads and writes pairing state in NVS, under a namespace of its
 * own so it never collides with NvsCredentialSource's or
 * NvsMessageStorage's keys.
 *
 * The pairing code is generated once, by ensureCode(), and persisted so
 * it stays the same across reboots until pairing completes: a code that
 * changed on every restart would leave the recipient reading a stale
 * value off the e-paper.
 */
class NvsPairingSource : public PairingSource {
public:
    const char* pairedChatId() const override;
    const char* pairingCode() const override;
    void bind(const char* chatId) override;

    /**
     * @brief Generates and persists a pairing code if none exists yet.
     *
     * Called once at boot so pairingCode() can stay a pure read: an
     * accessor that silently generated and wrote a code the first time
     * it was called would be surprising, and boot is a natural place to
     * guarantee one exists before anything (the e-paper, the portal)
     * asks for it.
     */
    void ensureCode();

private:
    /**
     * @brief Back the pointers returned by pairedChatId() and
     * pairingCode(), whose caller expects them to stay valid for the
     * call's duration.
     */
    mutable char chatId_[32] = {};
    mutable char code_[7] = {};
};
