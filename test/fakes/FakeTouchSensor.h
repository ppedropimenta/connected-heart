#pragma once

#include "Touch/TouchSensor.h"

class FakeTouchSensor : public TouchSensor {
public:
    void begin() override {}
    void update() override {}

    bool wasPressed() override {
        const bool pressed = pending_;
        pending_ = false;
        return pressed;
    }

    /**
     * @brief Simulates a debounced press, reported on the next wasPressed().
     */
    void press() {
        pending_ = true;
    }

private:
    bool pending_ = false;
};
