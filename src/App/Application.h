/**
* @file Application.h
 * @brief Defines the main application controller.
 *
 * The Application class owns the system modules and coordinates
 * their initialization and periodic execution.
 */

#pragma once

#include "Display/EpaperDisplay.h"
#include "Led/Led.h"
#include "../Touch/TouchSensor.h"
#include "../Message/MessageManager.h"
#include "../State/DeviceState.h"
#include "../State/StateMachine.h"
#include "../State/DeviceEvent.h"
#include "../Communication/SerialMessageSource.h"

class Application {
public:
    void begin();
    void update();

private:
    void processInputs();
    void processTimers();
    void processEvent(DeviceEvent event);

    void enterState(DeviceState state);

    StateMachine stateMachine_;
    MessageManager messageManager_;

    EpaperDisplay display_;
    Led led_;
    TouchSensor touch_;

    unsigned long stateEnteredAt_ = 0;
};
