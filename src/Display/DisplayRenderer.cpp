#include "DisplayRenderer.h"

#include <Core/Log.h>

DisplayRenderer::DisplayRenderer(
    EpaperDisplay& display
)
    : display_(display)
{
}

void DisplayRenderer::showStartup()
{
    LOG_DEBUG("Rendering startup screen");

    display_.render([this]()
    {
        display_.setBoldFont();

        display_.drawCenteredText(
            "Connected Heart",
            display_.width() / 2,
            45
        );

        display_.setRegularFont();

        display_.drawCenteredText(
            "Starting...",
            display_.width() / 2,
            78
        );
    });
}

void DisplayRenderer::showConnecting()
{
    LOG_DEBUG("Rendering connecting screen");

    display_.render([this]()
    {
        display_.drawHeart(
            display_.width() / 2,
            35
        );

        display_.setRegularFont();

        display_.drawCenteredText(
            "Connecting...",
            display_.width() / 2,
            82
        );
    });
}

void DisplayRenderer::showWaitingForMessage(
    const ConnectionState connectionState
)
{
    LOG_DEBUG("Rendering waiting screen");

    display_.render([this, connectionState]()
    {
        if (connectionState == ConnectionState::Error)
        {
            display_.drawHeart(
                display_.width() / 2,
                30
            );

            display_.setBoldFont();

            display_.drawCenteredText(
                "No connection",
                display_.width() / 2,
                66
            );

            display_.setRegularFont();

            display_.drawCenteredText(
                "Reconnecting...",
                display_.width() / 2,
                92
            );

            drawConnectionIndicatorIfNeeded(connectionState);

            return;
        }

        display_.drawHeart(
            display_.width() / 2,
            32
        );

        display_.setRegularFont();

        display_.drawCenteredWrappedText(
            "Esperando uma mensagem...",
            display_.width() / 2,
            82,
            display_.width() - 30,
            2,
            20
        );

        drawConnectionIndicatorIfNeeded(connectionState);
    });
}

void DisplayRenderer::showNotification(
    const ConnectionState connectionState
)
{
    LOG_DEBUG("Rendering notification screen");

    display_.render([this, connectionState]()
    {
        display_.setBoldFont();

        display_.drawCenteredText(
            "Uma mensagem chegou!",
            display_.width() / 2,
            45
        );

        display_.setRegularFont();

        display_.drawCenteredText(
            "toque para ver",
            display_.width() / 2,
            78
        );

        drawConnectionIndicatorIfNeeded(connectionState);
    });
}

void DisplayRenderer::showRevealHeart(
    const ConnectionState connectionState
)
{
    LOG_DEBUG("Rendering reveal screen");

    display_.render([this, connectionState]()
    {
        display_.drawHeart(
            display_.width() / 2,
            display_.height() / 2
        );

        drawConnectionIndicatorIfNeeded(connectionState);
    });
}

void DisplayRenderer::showMessage(
    const String& text,
    const ConnectionState connectionState
)
{
    LOG_DEBUG("Rendering message screen");

    display_.render([this, &text, connectionState]()
    {
        display_.setRegularFont();

        /*
         * Reserve some horizontal room so the text does not collide with
         * the connection indicator.
         */
        constexpr int16_t horizontalMargin = 18;

        display_.drawCenteredWrappedText(
            text,
            display_.width() / 2,
            display_.height() / 2,
            display_.width() - (horizontalMargin * 2),
            5,
            20
        );

        drawConnectionIndicatorIfNeeded(connectionState);
    });
}

void DisplayRenderer::drawConnectionIndicatorIfNeeded(
    const ConnectionState connectionState
)
{
    if (connectionState != ConnectionState::Error)
    {
        return;
    }

    display_.drawConnectionErrorIndicator();
}