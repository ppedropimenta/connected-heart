/**
* @file PairingSource.h
 * @brief Interface for supplying the paired sender's chat identifier that
 * TelegramMessageSource filters incoming updates against, and for claiming
 * that identifier through pairing by code (ADR-0004).
 *
 * This is the seam behind which a compiled-in path
 * (CompiledPairingSource, for bench work) and the pairing-by-code flow
 * that learns the identifier at runtime (NvsPairingSource) are
 * interchangeable, mirroring CredentialSource / CompiledCredentialSource
 * for WiFi. TelegramMessageSource depends on this interface and never on a
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

    /**
     * @brief The code an unpaired heart expects from its Sender, shown on
     * both the configuration page and the e-paper during Setup.
     *
     * Meaningless once paired: CompiledPairingSource, whose bench builds
     * are already paired via a compiled chat identifier, never generates
     * one and this value is never shown or checked in that path.
     */
    virtual const char* pairingCode() const = 0;

    /**
     * @brief Claims the given chat as the paired Sender.
     *
     * Called by TelegramMessageSource once an unpaired chat's message has
     * matched pairingCode(). No-op for CompiledPairingSource: a bench
     * build is already paired, so this is never reached in practice.
     */
    virtual void bind(const char* chatId) = 0;

    /**
     * @brief Whether a Sender has already been claimed.
     */
    bool isPaired() const {
        return pairedChatId()[0] != '\0';
    }
};
