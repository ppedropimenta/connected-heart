#include "NeoPixelLed.h"

#include <Arduino.h>

#include "Core/Config.h"
#include "Core/Pins.h"

namespace {
    constexpr uint16_t PIXEL_COUNT = 3;
    constexpr uint8_t FULL_BRIGHTNESS = 255;

    /*
     * Deliberately dim: NotificationPulse runs unattended for hours,
     * potentially in a dark bedroom. Provisional pending hardware tuning,
     * per ticket 06.
     */
    constexpr uint8_t NOTIFICATION_MAX_BRIGHTNESS = 40;

    /**
     * @brief Triangle wave rising then falling once per period, scaled to
     * maxBrightness.
     */
    uint8_t breathe(
        const unsigned long elapsedMs,
        const unsigned long periodMs,
        const uint8_t maxBrightness
    ) {
        const unsigned long half = periodMs / 2;
        const unsigned long phase = elapsedMs % periodMs;
        const unsigned long level = phase < half ? phase : (periodMs - phase);

        return static_cast<uint8_t>((level * maxBrightness) / half);
    }

    /**
     * @brief On for the first onMs of every period, off for the rest.
     */
    bool squareWave(
        const unsigned long elapsedMs,
        const unsigned long periodMs,
        const unsigned long onMs
    ) {
        return (elapsedMs % periodMs) < onMs;
    }

    /**
     * @brief Two quick brightness pulses ("lub-dub") followed by a rest,
     * mimicking a heartbeat.
     */
    uint8_t doubleThump(
        const unsigned long elapsedMs,
        const unsigned long periodMs,
        const uint8_t maxBrightness
    ) {
        const unsigned long phase = elapsedMs % periodMs;
        const unsigned long thumpMs = periodMs / 6;
        const unsigned long gapMs = thumpMs / 2;
        const unsigned long secondThumpStart = thumpMs + gapMs;

        if (phase < thumpMs) {
            return breathe(phase, thumpMs, maxBrightness);
        }

        if (phase >= secondThumpStart && phase < secondThumpStart + thumpMs) {
            return breathe(phase - secondThumpStart, thumpMs, maxBrightness);
        }

        return 0;
    }
}

NeoPixelLed::NeoPixelLed()
    : pixel_(PIXEL_COUNT, Pins::NotificationLed::LED_DATA, NEO_GRB + NEO_KHZ800)
{
}

void NeoPixelLed::begin() {
    pixel_.begin();
    pixel_.clear();
    pixel_.show();
}

void NeoPixelLed::show(const LedPattern pattern) {
    if (pattern == pattern_) {
        return;
    }

    pattern_ = pattern;
    patternStartedAt_ = millis();

    render();
}

void NeoPixelLed::update() {
    render();
}

/**
 * @brief Sets every pixel in the chain to a colour whose channels are
 * scaled by brightness out of a fixed 0-255 range.
 *
 * brightness is always relative to full scale rather than to a pattern's
 * own cap, so a dim pattern (a low brightness ceiling passed to breathe(),
 * squareWave(), or doubleThump()) actually stays dim instead of the cap
 * cancelling out against itself.
 */
void NeoPixelLed::setColorAtBrightness(
    const uint8_t maxRed,
    const uint8_t maxGreen,
    const uint8_t maxBlue,
    const uint8_t brightness
) {
    pixel_.fill(Adafruit_NeoPixel::Color(
        static_cast<uint8_t>((static_cast<uint16_t>(maxRed) * brightness) / FULL_BRIGHTNESS),
        static_cast<uint8_t>((static_cast<uint16_t>(maxGreen) * brightness) / FULL_BRIGHTNESS),
        static_cast<uint8_t>((static_cast<uint16_t>(maxBlue) * brightness) / FULL_BRIGHTNESS)
    ));
}

void NeoPixelLed::render() {
    const unsigned long elapsedMs = millis() - patternStartedAt_;

    switch (pattern_) {
        case LedPattern::Off:
            pixel_.clear();
            break;

        case LedPattern::Setup:
            setColorAtBrightness(
                0, 0, 255,
                breathe(elapsedMs, Config::LedSetupBreathePeriodMs, FULL_BRIGHTNESS)
            );
            break;

        case LedPattern::Connecting:
            setColorAtBrightness(
                0, 0, 255,
                squareWave(elapsedMs, Config::LedConnectingBlinkPeriodMs, Config::LedConnectingBlinkPeriodMs / 3)
                    ? FULL_BRIGHTNESS
                    : 0
            );
            break;

        case LedPattern::NotificationPulse:
            setColorAtBrightness(
                255, 25, 0,
                breathe(elapsedMs, Config::LedPulsePeriodMs, NOTIFICATION_MAX_BRIGHTNESS)
            );
            break;

        case LedPattern::RevealHeartbeat:
            setColorAtBrightness(
                255, 25, 0,
                doubleThump(elapsedMs, Config::LedRevealThumpPeriodMs, FULL_BRIGHTNESS)
            );
            break;

        case LedPattern::ConnectionError:
            setColorAtBrightness(
                255, 140, 0,
                squareWave(elapsedMs, Config::LedConnectionErrorBlinkPeriodMs, Config::LedConnectionErrorBlinkOnMs)
                    ? FULL_BRIGHTNESS
                    : 0
            );
            break;
    }

    pixel_.show();
}
