/**
* @file ResetButton.h
 * @brief Debounced GPIO reset button that reports a long press.
 */

#pragma once

#include "Core/Module.h"

/**
 * @brief Reads the reset button wired to Pins::ResetButton::BUTTON and
 * detects a long press.
 *
 * Assumes an active-low button wired to an internal pull-up (pressed pulls
 * the pin LOW). update() must be called frequently enough that
 * Config::ButtonDebounceMs and Config::ResetButtonHoldMs are many calls,
 * not a handful.
 *
 * This is a thin hardware adapter, not the seam Application depends on:
 * unlike TouchSensor, a factory reset clears persisted stores that
 * Application does not itself own (WiFi credentials, pairing), so main.cpp
 * drives it directly, the same way it drives WifiManager.
 */
class ResetButton final : public Module {
public:
    void begin() override;
    void update() override;

    /**
     * @brief Whether the button has just completed a long press.
     *
     * Reported at most once per press: the button must be released before
     * another long press can be reported.
     */
    bool wasLongPressed();

private:
    [[nodiscard]]
    bool rawPressed() const;

    bool stablePressed_ = false;
    bool lastRawPressed_ = false;
    unsigned long lastChangeAt_ = 0;
    unsigned long pressedAt_ = 0;
    bool longPressReported_ = false;
    bool pending_ = false;
};
