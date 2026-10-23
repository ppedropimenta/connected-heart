#pragma once

#include "Led/Led.h"

class FakeLed : public Led {
public:
    void begin() override {}
    void update() override {}

    void show(const LedPattern pattern) override {
        lastPattern = pattern;
    }

    LedPattern lastPattern = LedPattern::Off;
};
