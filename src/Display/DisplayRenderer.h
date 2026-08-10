#pragma once

#include <Arduino.h>

#include "EpaperDisplay.h"

#include <State/ConnectionState.h>

/**
 * @brief Renders the Connected Heart user interface.
 *
 * DisplayRenderer knows what each product screen should look like.
 *
 * It does not decide when screens change and does not access
 * MessageManager or StateMachine directly.
 */
class DisplayRenderer
{
public:
    explicit DisplayRenderer(EpaperDisplay& display);

    /**
     * @brief Displays the startup screen.
     */
    void showStartup();

    /**
     * @brief Displays the initial connection screen.
     */
    void showConnecting();

    /**
     * @brief Displays the normal waiting screen.
     */
    void showWaitingForMessage(
        ConnectionState connectionState
    );

    /**
     * @brief Displays the new-message notification.
     */
    void showNotification(
        ConnectionState connectionState
    );

    /**
     * @brief Displays the heart reveal screen.
     */
    void showRevealHeart(
        ConnectionState connectionState
    );

    /**
     * @brief Displays the final resolved message text.
     *
     * The caller is responsible for resolving predefined HeartMessage
     * instances into their final Portuguese text before calling this method.
     */
    void showMessage(
        const String& text,
        ConnectionState connectionState
    );

private:
    EpaperDisplay& display_;

    void drawConnectionIndicatorIfNeeded(
        ConnectionState connectionState
    );
};