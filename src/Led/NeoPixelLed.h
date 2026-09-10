/**
* @file NeoPixelLed.h
 * @brief Led rendering each LedPattern as a colour and rhythm on a single
 * NeoPixel.
 */

#pragma once

#include <Adafruit_NeoPixel.h>

#include "Led/Led.h"

/**
 * @brief Drives the notification NeoPixel from whichever LedPattern was
 * last shown.
 *
 * update() must be called frequently enough that each rhythm's shortest
 * interval is many calls, not one.
 */
class NeoPixelLed final : public Led {
public:
    NeoPixelLed();

    void begin() override;
    void update() override;
    void show(LedPattern pattern) override;

private:
    void render();
    void setColorAtBrightness(
        uint8_t maxRed,
        uint8_t maxGreen,
        uint8_t maxBlue,
        uint8_t brightness
    );

    Adafruit_NeoPixel pixel_;
    LedPattern pattern_ = LedPattern::Off;
    unsigned long patternStartedAt_ = 0;
};
