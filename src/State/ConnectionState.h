#pragma once

/**
 * @brief Represents the device connection status independently from the
 * main UX state.
 *
 * This allows the display to combine, for example, a displayed message with
 * a connection-error indicator.
 */
enum class ConnectionState {
    /**
     * A connection attempt is currently in progress.
     */
    Connecting,

    /**
     * The required network and message services are available.
     */
    Connected,

    /**
     * The connection is currently unavailable.
     *
     * Reconnection may happen automatically or be requested through touch.
     */
    Error
};