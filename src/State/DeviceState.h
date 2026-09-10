#pragma once

/**
 * @brief Represents the current stage of the Connected Heart UX.
 *
 * Connection status is modeled separately through ConnectionState.
 */
enum class DeviceState {
    /**
     * Hardware and software modules are being initialized.
     */
    Boot,

    /**
     * The device has no usable network or Pairing and offers its own
     * access point so it can be provisioned.
     */
    Setup,

    /**
    * The device is performing its initial connection process.
    *
    * This state belongs to the product flow because the device is not yet
    * ready to wait for messages.
    */
    Waking,

    /**
     * The device is ready and no message is waiting to be revealed.
     */
    WaitingForMessage,

    /**
     * A message is pending and the device is waiting for a touch.
     */
    Notification,

    /**
     * The current message is going through its reveal animation.
     */
    RevealingMessage,

    /**
     * The current message is displayed.
     */
    DisplayingMessage
};