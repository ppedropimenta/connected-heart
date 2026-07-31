#pragma once

/**
 * @brief Events accepted by the main Connected Heart state machine.
 *
 * Network failures and reconnection attempts are not represented here.
 * They belong to the separate connection state.
 */
enum class DeviceEvent {
    /**
     * All required device modules finished their initialization.
     */
    InitializationFinished,

    /**
     * The initial connection process completed successfully.
     */
    ConnectionEstablished,

    /**
     * A valid message was received and stored by MessageManager.
     */
    MessageReceived,

    /**
     * A valid touch interaction was detected.
     */
    TouchPressed,

    /**
     * The visual reveal sequence finished.
     */
    RevealFinished
};
