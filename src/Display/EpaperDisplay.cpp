#include "EpaperDisplay.h"

#include <Display/EpaperDisplay.h>

#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include <Core/Log.h>
#include <Core/Pins.h>

EpaperDisplay::EpaperDisplay()
    : display_(
        GxEPD2_213_BN(
            Pins::Display::CS,
            Pins::Display::DC,
            Pins::Display::RST,
            Pins::Display::BUSY
        )
    )
{
}

void EpaperDisplay::begin()
{
    LOG_INFO("Initializing e-paper display");

    display_.init(
        115200,
        true,
        50,
        false
    );

    display_.setRotation(1);
    display_.setTextColor(GxEPD_BLACK);

    initialized_ = true;

    showStartupScreen();

    LOG_INFO("E-paper display initialized");
}

void EpaperDisplay::update()
{
    /*
     * The e-paper display does not require continuous updates.
     *
     * Future display state changes can be processed here, for example:
     *
     * - pending screen changes;
     * - timed refreshes;
     * - battery status updates;
     * - network connection status updates.
     */
}

void EpaperDisplay::showStartupScreen()
{
    if (!initialized_)
    {
        LOG_WARN("Cannot show startup screen: display not initialized");
        return;
    }

    prepareDrawing();

    display_.firstPage();

    do
    {
        display_.fillScreen(GxEPD_WHITE);

        display_.drawRect(
            4,
            4,
            display_.width() - 8,
            display_.height() - 8,
            GxEPD_BLACK
        );

        display_.setFont(&FreeMonoBold9pt7b);

        drawCenteredText(
            "Connected Heart",
            display_.width() / 2,
            45
        );

        display_.setFont(&FreeMono9pt7b);

        drawCenteredText(
            "Starting...",
            display_.width() / 2,
            75
        );
    }
    while (display_.nextPage());
}

void EpaperDisplay::showMessage(
    const char* title,
    const char* message
)
{
    if (!initialized_)
    {
        LOG_WARN("Cannot show message: display not initialized");
        return;
    }

    if (title == nullptr || message == nullptr)
    {
        LOG_ERROR("Cannot show message: null text");
        return;
    }

    prepareDrawing();

    display_.firstPage();

    constexpr int titleCenterY = 30;
    constexpr int dividerY = 45;
    constexpr int messageCenterY = 80;

    do
    {
        display_.fillScreen(GxEPD_WHITE);

        display_.setFont(&FreeMonoBold9pt7b);

        drawCenteredText(
            title,
            display_.width() / 2,
            titleCenterY
        );

        display_.drawFastHLine(
            15,
            dividerY,
            display_.width() - 30,
            GxEPD_BLACK
        );

        display_.setFont(&FreeMono9pt7b);

        drawCenteredText(
            message,
            display_.width() / 2,
            messageCenterY
        );
    }
    while (display_.nextPage());
}

void EpaperDisplay::clear()
{
    if (!initialized_)
    {
        LOG_WARN("Cannot clear display: display not initialized");
        return;
    }

    prepareDrawing();

    display_.firstPage();

    do
    {
        display_.fillScreen(GxEPD_WHITE);
    }
    while (display_.nextPage());
}

bool EpaperDisplay::isInitialized() const
{
    return initialized_;
}

void EpaperDisplay::prepareDrawing()
{
    display_.setRotation(1);
    display_.setTextColor(GxEPD_BLACK);
    display_.setFullWindow();
}

void EpaperDisplay::drawCenteredText(
    const char* text,
    const int16_t centerX,
    const int16_t centerY
)
{
    if (text == nullptr)
    {
        return;
    }

    int16_t boundsX = 0;
    int16_t boundsY = 0;

    uint16_t boundsWidth = 0;
    uint16_t boundsHeight = 0;

    display_.getTextBounds(
        text,
        0,
        0,
        &boundsX,
        &boundsY,
        &boundsWidth,
        &boundsHeight
    );

    const int16_t cursorX =
        centerX - static_cast<int16_t>(boundsWidth / 2) - boundsX;

    const int16_t cursorY =
        centerY - static_cast<int16_t>(boundsHeight / 2) - boundsY;

    display_.setCursor(
        cursorX,
        cursorY
    );

    display_.print(text);
}