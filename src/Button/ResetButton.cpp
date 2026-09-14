#include "ResetButton.h"

#include <Arduino.h>

#include "Core/Config.h"
#include "Core/Log.h"
#include "Core/Pins.h"

void ResetButton::begin() {
    pinMode(Pins::ResetButton::BUTTON, INPUT_PULLUP);

    stablePressed_ = rawPressed();
    lastRawPressed_ = stablePressed_;
    lastChangeAt_ = millis();

    if (stablePressed_) {
        pressedAt_ = lastChangeAt_;
    }
}

void ResetButton::update() {
    const bool raw = rawPressed();

    if (raw != lastRawPressed_) {
        lastRawPressed_ = raw;
        lastChangeAt_ = millis();
    }

    if (raw != stablePressed_ && millis() - lastChangeAt_ >= Config::ButtonDebounceMs) {
        stablePressed_ = raw;
        longPressReported_ = false;

        if (stablePressed_) {
            pressedAt_ = millis();
        }
    }

    if (stablePressed_ && !longPressReported_ && millis() - pressedAt_ >= Config::ResetButtonHoldMs) {
        LOG_DEBUG("Reset button long press detected");
        longPressReported_ = true;
        pending_ = true;
    }
}

bool ResetButton::wasLongPressed() {
    const bool value = pending_;
    pending_ = false;
    return value;
}

bool ResetButton::rawPressed() const {
    return digitalRead(Pins::ResetButton::BUTTON) == LOW;
}
