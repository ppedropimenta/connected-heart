#pragma once

#include "Core/Clock.h"

class FakeClock : public Clock {
public:
    unsigned long millis() const override {
        return now_;
    }

    void advance(const unsigned long ms) {
        now_ += ms;
    }

private:
    unsigned long now_ = 0;
};
