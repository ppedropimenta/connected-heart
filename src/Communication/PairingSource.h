/**
* @file PairingSource.h
 * @brief Interface for supplying the paired sender's chat identifier that
 * TelegramMessageSource filters incoming updates against.
 *
 * This is the seam behind which a compiled-in path
 * (CompiledPairingSource, for bench work) and the pairing-by-code flow
 * that learns the identifier at runtime (ticket 12) are interchangeable,
 * mirroring CredentialSource / CompiledCredentialSource for WiFi.
 * TelegramMessageSource depends on this interface and never on a
 * concrete source.
 */

#pragma once

class PairingSource {
public:
    virtual ~PairingSource() = default;

    /**
     * @brief The paired sender's chat identifier.
     *
     * @return The paired chat identifier, or an empty string if the
     * heart is not yet paired. An unpaired heart therefore matches no
     * chat and accepts nothing.
     */
    virtual const char* pairedChatId() const = 0;
};
