#pragma once

#include <Arduino.h>

#include "Display/DisplayRenderer.h"
#include "Display/EpaperDisplay.h"

/**
 * @brief Renders the Connected Heart user interface on the physical
 * e-paper display.
 */
class EpaperDisplayRenderer final : public DisplayRenderer
{
public:
    explicit EpaperDisplayRenderer(EpaperDisplay& display);

    void showStartup() override;
    void showConnecting() override;

    void showWaitingForMessage(
        ConnectionState connectionState
    ) override;

    void showNotification(
        ConnectionState connectionState
    ) override;

    void showRevealHeart(
        ConnectionState connectionState
    ) override;

    void showMessage(
        const char* text,
        ConnectionState connectionState
    ) override;

private:
    EpaperDisplay& display_;

    void drawConnectionIndicatorIfNeeded(
        ConnectionState connectionState
    );
};
