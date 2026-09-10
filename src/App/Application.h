/**
* @file Application.h
 * @brief Defines the main application controller.
 *
 * Application owns the product's flow: it translates inputs from its
 * injected collaborators into DeviceEvents, feeds them to StateMachine,
 * and drives the renderer and LED whenever the state changes.
 *
 * All hardware access happens through injected interfaces, so the whole
 * flow can be exercised on the host with fakes standing in for the
 * renderer, the LED, the touch input, the message source, and the clock.
 */

#pragma once

#include "Communication/MessageSource.h"
#include "Core/Clock.h"
#include "Display/DisplayRenderer.h"
#include "Led/Led.h"
#include "Message/MessageManager.h"
#include "State/DeviceEvent.h"
#include "State/DeviceState.h"
#include "State/StateMachine.h"
#include "Touch/TouchSensor.h"

class Application {
public:
    Application(
        DisplayRenderer& renderer,
        Led& led,
        TouchSensor& touch,
        MessageSource& messageSource,
        Clock& clock
    );

    void begin();
    void update();

private:
    void processInputs();
    void processTimers();
    void processEvent(DeviceEvent event);

    void enterState(DeviceState state);

    DisplayRenderer& renderer_;
    Led& led_;
    TouchSensor& touch_;
    MessageSource& messageSource_;
    Clock& clock_;

    StateMachine stateMachine_;
    MessageManager messageManager_;

    unsigned long stateEnteredAt_ = 0;
};
