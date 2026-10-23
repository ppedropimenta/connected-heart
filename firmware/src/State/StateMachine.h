#pragma once

#include "DeviceEvent.h"
#include "DeviceState.h"

/**
 * @brief Controls the main UX flow.
 *
 * The state machine only decides whether an event is valid and which
 * DeviceState should follow.
 *
 * It does not:
 *
 * - access MessageManager;
 * - control the display;
 * - control the notification LED;
 * - read the touch sensor;
 * - manage Wi-Fi or Telegram;
 * - manage reveal timers.
 */
class StateMachine {
public:
    /**
     * @brief Creates a state machine in DeviceState::Boot.
     */
    StateMachine() = default;
    
    DeviceState state() const;

    /**
     * @brief Processes an event according to the current state.
     *
     * @param event Event to process.
     * @param provisioned Whether the heart already has a usable network and
     * Pairing. Only consulted when processing InitializationFinished from
     * Boot, to decide between Waking and Setup; ignored otherwise.
     *
     * @return true if the event is valid in the current state.
     * @return false if the event is ignored.
     *
     * A valid event can return true even when the state remains unchanged.
     * For example, receiving another message while already in Notification
     * replaces the pending message but keeps the device in Notification.
     */
    bool process(DeviceEvent event, bool provisioned);

    /**
     * @brief Restores the state machine to its initial state.
     *
     * Any internally stored deferred transition is also removed.
     */
    void reset();

private:
    /**
     * @brief Current stage of the main product flow.
     */
    DeviceState state_ = DeviceState::Boot;

    /**
     * @brief Indicates that another message arrived while the current
     * message was being revealed.
     *
     * When the reveal finishes, the device returns directly to Notification
     * instead of DisplayingMessage.
     */
    bool messageReceivedDuringReveal_ = false;
};