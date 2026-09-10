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
    RevealFinished,

    /**
     * Setup completed and the heart has a network and a Sender to use.
     */
    ProvisioningFinished,

    /**
     * A configured number of consecutive connection attempts have failed.
     *
     * Driven by a count of consecutive failures, not by elapsed time alone,
     * so a router that is slow to answer is not treated the same as one
     * that is gone.
     */
    ConnectionAttemptsExhausted
};
