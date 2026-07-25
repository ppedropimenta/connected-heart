/**
* @file Led.h
 * @brief Controls the onboard status LED.
 */

#pragma once

#include "Core/Module.h"

class Led final : public Module {
public:
    enum class State {
        Off,
        On
    };

    void begin() override;
    void update() override;

    /**
     * @brief Turns the LED on.
     */
    void on();

    /**
     * @brief Turns the LED off.
     */
    void off();

    /**
     * @brief Toggles the LED state.
     */
    void toggle();

    void setState(State state);

    /**
     * @brief Returns whether the LED is currently on.
     */
    [[nodiscard]] State getState() const;

private:
    State state_ = State::Off;
};
