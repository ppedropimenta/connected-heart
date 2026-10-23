#pragma once

#include "State/ConnectionState.h"

/**
 * @brief Renders the Connected Heart user interface.
 *
 * DisplayRenderer knows what each product screen should look like.
 *
 * It does not decide when screens change and does not access
 * MessageManager or StateMachine directly.
 *
 * This is the renderer seam Application depends on. The low-level e-paper
 * class cannot be the seam because its render method is a template and
 * therefore cannot be virtual; EpaperDisplayRenderer implements this
 * interface on top of it instead.
 */
class DisplayRenderer
{
public:
    virtual ~DisplayRenderer() = default;

    /**
     * @brief Displays the startup screen.
     */
    virtual void showStartup() = 0;

    /**
     * @brief Displays the initial connection screen.
     */
    virtual void showConnecting() = 0;

    /**
     * @brief Displays the setup screen for a heart with no usable network
     * or Pairing.
     *
     * @param apSsid Name of the access point the heart is offering, so the
     * recipient knows which network to join.
     * @param pairingCode The code the Sender must send the bot to claim
     * this heart (ADR-0004). An empty string means the heart is
     * already paired (this is a reconnect, not a fresh setup), in which
     * case no pairing code section should be shown at all.
     */
    virtual void showSetup(const char* apSsid, const char* pairingCode) = 0;

    /**
     * @brief Displays the normal waiting screen.
     */
    virtual void showWaitingForMessage(
        ConnectionState connectionState
    ) = 0;

    /**
     * @brief Displays the new-message notification.
     */
    virtual void showNotification(
        ConnectionState connectionState
    ) = 0;

    /**
     * @brief Displays the heart reveal screen.
     */
    virtual void showRevealHeart(
        ConnectionState connectionState
    ) = 0;

    /**
     * @brief Displays the final resolved message text.
     *
     * The caller is responsible for resolving predefined HeartMessage
     * instances into their final Portuguese text before calling this method.
     */
    virtual void showMessage(
        const char* text,
        ConnectionState connectionState
    ) = 0;
};
