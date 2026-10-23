#include "EpaperDisplayRenderer.h"

#include <Core/Log.h>

EpaperDisplayRenderer::EpaperDisplayRenderer(
    EpaperDisplay& display
)
    : display_(display)
{
}

void EpaperDisplayRenderer::showStartup()
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

void EpaperDisplayRenderer::showConnecting()
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

void EpaperDisplayRenderer::showSetup(const char* apSsid, const char* pairingCode)
{
    LOG_DEBUG("Rendering setup screen");

    const String ssid(apSsid);
    const String code(pairingCode);

    /*
     * An empty code means the heart is already paired and this Setup
     * visit is ADR-0004's automatic recovery, not a fresh setup: showing
     * a pairing code there would wrongly suggest re-pairing is needed.
     */
    const bool showCode = code.length() > 0;

    display_.render([this, &ssid, &code, showCode]()
    {
        display_.setBoldFont();

        /*
         * An already-paired heart landing here arrived through ADR-0004's
         * automatic recovery (too many failed connection attempts), not a
         * fresh setup: the heading and instructions say so, rather than
         * silently reusing the first-setup copy as if nothing happened.
         */
        display_.drawCenteredText(
            showCode ? "Configuracao" : "Conexao perdida",
            display_.width() / 2,
            showCode ? 18 : 38
        );

        display_.setRegularFont();

        display_.drawCenteredText(
            showCode ? "Conecte-se a rede:" : "Reconecte-se a rede:",
            display_.width() / 2,
            showCode ? 40 : 68
        );

        display_.setBoldFont();

        display_.drawCenteredText(
            ssid,
            display_.width() / 2,
            showCode ? 60 : 94
        );

        if (!showCode) {
            return;
        }

        display_.setRegularFont();

        display_.drawCenteredText(
            "Codigo de pareamento:",
            display_.width() / 2,
            84
        );

        display_.setBoldFont();

        display_.drawCenteredText(
            code,
            display_.width() / 2,
            106
        );
    });
}

void EpaperDisplayRenderer::showWaitingForMessage(
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

void EpaperDisplayRenderer::showNotification(
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

void EpaperDisplayRenderer::showRevealHeart(
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

void EpaperDisplayRenderer::showMessage(
    const char* text,
    const ConnectionState connectionState
)
{
    LOG_DEBUG("Rendering message screen");

    const String message(text);

    display_.render([this, &message, connectionState]()
    {
        display_.setRegularFont();

        /*
         * Reserve some horizontal room so the text does not collide with
         * the connection indicator.
         */
        constexpr int16_t horizontalMargin = 18;

        display_.drawCenteredWrappedText(
            message,
            display_.width() / 2,
            display_.height() / 2,
            display_.width() - (horizontalMargin * 2),
            5,
            20
        );

        drawConnectionIndicatorIfNeeded(connectionState);
    });
}

void EpaperDisplayRenderer::drawConnectionIndicatorIfNeeded(
    const ConnectionState connectionState
)
{
    if (connectionState != ConnectionState::Error)
    {
        return;
    }

    display_.drawConnectionErrorIndicator();
}
