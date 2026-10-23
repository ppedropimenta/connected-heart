/**
* @file ArduinoClock.h
 * @brief Clock backed by Arduino's own millis().
 */

#pragma once

#include <Arduino.h>

#include "Core/Clock.h"

class ArduinoClock final : public Clock {
public:
    unsigned long millis() const override {
        return ::millis();
    }
};
