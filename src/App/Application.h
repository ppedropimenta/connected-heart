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
#include "Storage/MessageStorage.h"
#include "Touch/TouchSensor.h"

class Application {
public:
    Application(
        DisplayRenderer& renderer,
        Led& led,
        TouchSensor& touch,
        MessageSource& messageSource,
        MessageStorage& storage,
        Clock& clock
    );

    /**
     * @brief Runs the device from DeviceState::Boot.
     *
     * Restores the Current Message from storage, if one was persisted
     * before the last power cycle, before the state machine is fed its
     * first event.
     *
     * @param provisioned Whether the heart already has a usable network and
     * Pairing. Chooses whether Boot leads to Waking or Setup.
     */
    void begin(bool provisioned);

    void update();

    /**
     * @brief Reports that Setup has produced a usable network and Pairing.
     *
     * Called once the captive portal and pairing have both completed.
     */
    void notifyProvisioningFinished();

    /**
     * @brief Reports that the heart's connection is up.
     *
     * Also clears the consecutive-failure count kept for
     * DeviceEvent::ConnectionAttemptsExhausted.
     */
    void notifyConnectionEstablished();

    /**
     * @brief Reports that a single connection attempt has failed.
     *
     * Once Config::MaxConsecutiveConnectionFailures consecutive failures
     * have been reported, raises DeviceEvent::ConnectionAttemptsExhausted.
     */
    void notifyConnectionAttemptFailed();

private:
    void processInputs();
    void processTimers();

    /**
     * @brief Feeds an event to StateMachine and drives the renderer/LED on
     * a resulting state change.
     *
     * @param provisioned Forwarded to StateMachine::process(); meaningful
     * only for InitializationFinished, ignored for every other event.
     *
     * @return true if the event was valid in the current state.
     */
    bool processEvent(DeviceEvent event, bool provisioned = true);

    void enterState(DeviceState state);

    /**
     * @brief Resolves and shows the Current Message.
     *
     * Used both for DeviceState::DisplayingMessage and for
     * DeviceState::WaitingForMessage when a message was restored from
     * storage, so a recipient never sees the waiting screen while a
     * message is available to show.
     */
    void showCurrentMessage();

    DisplayRenderer& renderer_;
    Led& led_;
    TouchSensor& touch_;
    MessageSource& messageSource_;
    MessageStorage& storage_;
    Clock& clock_;

    StateMachine stateMachine_;
    MessageManager messageManager_;

    unsigned long stateEnteredAt_ = 0;
    unsigned int consecutiveConnectionFailures_ = 0;
};
