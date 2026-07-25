#include "Led.h"

#include <Arduino.h>

#include "Core/Pins.h"

void Led::begin()
{
    pinMode(Pins::STATUS_LED, OUTPUT);
    off();
}

void Led::update() {}

void Led::on() {
    setState(State::On);
}

void Led::off() {
    setState(State::Off);
}

void Led::toggle() {
    if (state_ == State::On) {
        off();
    }
    else {
        on();
    }
}

void Led::setState(State state) {
    state_ = state;

    digitalWrite(Pins::STATUS_LED, state == State::On ? LOW : HIGH);
}

Led::State Led::getState() const {
    return state_;
}
