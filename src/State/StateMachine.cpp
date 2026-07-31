#include "StateMachine.h"

DeviceState StateMachine::state() const {
    return state_;
}

bool StateMachine::process(const DeviceEvent event) {
    switch (state_) {
        case DeviceState::Boot:
        {
            if (event == DeviceEvent::InitializationFinished) {
                state_ = DeviceState::Connecting;
                return true;
            }

            return false;
        }

        case DeviceState::Connecting:
        {
            if (event == DeviceEvent::ConnectionEstablished) {
                state_ = DeviceState::WaitingForMessage;
                return true;
            }

            if (event == DeviceEvent::MessageReceived) {
                state_ = DeviceState::Notification;
                return true;
            }

            return false;
        }

        case DeviceState::WaitingForMessage:
        {
            if (event == DeviceEvent::MessageReceived) {
                state_ = DeviceState::Notification;
                return true;
            }

            return false;
        }

        case DeviceState::Notification:
        {
            if (event == DeviceEvent::MessageReceived) {
                /*
                 * The newly received message replaces the previous pending
                 * message in MessageManager.
                 *
                 * The event is valid, but no state transition is required.
                 */
                return true;
            }

            if (event == DeviceEvent::TouchPressed) {
                messageReceivedDuringReveal_ = false;
                state_ = DeviceState::RevealingMessage;
                return true;
            }

            return false;
        }

        case DeviceState::RevealingMessage:
        {
            if (event == DeviceEvent::MessageReceived) {
                /*
                 * The current message continues being revealed.
                 * The new message remains pending in MessageManager.
                 */
                messageReceivedDuringReveal_ = true;
                return true;
            }

            if (event == DeviceEvent::RevealFinished) {
                if (messageReceivedDuringReveal_) {
                    /*
                     * Another message is already pending, so the device
                     * immediately presents a new notification.
                     */
                    messageReceivedDuringReveal_ = false;
                    state_ = DeviceState::Notification;
                } else {
                    state_ = DeviceState::DisplayingMessage;
                }

                return true;
            }

            return false;
        }

        case DeviceState::DisplayingMessage:
        {
            if (event == DeviceEvent::MessageReceived) {
                state_ = DeviceState::Notification;
                return true;
            }

            /*
             * TouchPressed and all other events are intentionally ignored.
             * The displayed message remains visible until a new message
             * arrives.
             */
            return false;
        }
    }

    return false;
}

void StateMachine::reset() {
    state_ = DeviceState::Boot;
    messageReceivedDuringReveal_ = false;
}