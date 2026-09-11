/**
* @file GpioTouchSensor.h
 * @brief Debounced TouchSensor reading a digital touch module on the pin
 * configured as Pins::Touch::TOUCH.
 */

#pragma once

#include "Touch/TouchSensor.h"

/**
 * @brief Reads the touch surface's digital output and debounces it.
 *
 * Assumes an active-high touch module (HIGH while touched). update()
 * must be called frequently enough that Config::TouchDebounceMs is a
 * small number of calls, not a single long one.
 */
class GpioTouchSensor final : public TouchSensor {
public:
    void begin() override;
    void update() override;
    bool wasPressed() override;

private:
    [[nodiscard]]
    bool rawState() const;

    bool stableState_ = false;
    bool lastRawState_ = false;
    unsigned long lastChangeAt_ = 0;
    bool pending_ = false;
};
