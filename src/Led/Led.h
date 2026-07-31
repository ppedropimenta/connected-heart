/**
* @file Led.h
 * @brief Controls the onboard status LED.
 */

#pragma once

#include "Core/Module.h"
#include "Led/LedPattern.h"

class Led : public Module {
public:
    void begin() override;
    void update() override;

    void setPattern(LedPattern pattern);

private:
    LedPattern pattern_ = LedPattern::Off;
    unsigned long patternStartedAt_ = 0;
};