#include "GpioTouchSensor.h"

#include <Arduino.h>

#include "Core/Config.h"
#include "Core/Log.h"
#include "Core/Pins.h"

void GpioTouchSensor::begin() {
    pinMode(Pins::Touch::TOUCH, INPUT);

    stableState_ = rawState();
    lastRawState_ = stableState_;
    lastChangeAt_ = millis();
}

void GpioTouchSensor::update() {
    const bool raw = rawState();

    if (raw != lastRawState_) {
        lastRawState_ = raw;
        lastChangeAt_ = millis();
    }

    if (raw != stableState_ && millis() - lastChangeAt_ >= Config::TouchDebounceMs) {
        const bool isPress = raw && !stableState_;

        stableState_ = raw;

        if (isPress) {
            LOG_DEBUG("Touch press detected");
            pending_ = true;
        }
    }
}

bool GpioTouchSensor::wasPressed() {
    const bool pressed = pending_;
    pending_ = false;
    return pressed;
}

bool GpioTouchSensor::rawState() const {
    return digitalRead(Pins::Touch::TOUCH) == HIGH;
}
