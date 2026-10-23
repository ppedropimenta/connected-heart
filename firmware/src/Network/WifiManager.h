/**
* @file WifiManager.h
 * @brief Connects to WiFi and reports connectivity transitions.
 */

#pragma once

#include "Core/Module.h"
#include "CredentialSource.h"

/**
 * @brief Non-blocking WiFi connection, driven from CredentialSource.
 *
 * begin() starts the connection attempt and returns immediately; update()
 * must be polled regularly to advance it and detect transitions. While
 * disconnected, update() retries the connection on its own every
 * Config::WifiRetryIntervalMs and reports each try through attemptFailed(),
 * so a caller can count consecutive failures without treating every
 * millisecond of downtime as its own attempt.
 */
class WifiManager : public Module {
public:
    explicit WifiManager(CredentialSource& credentialSource);

    void begin() override;
    void update() override;

    bool isConnected() const;

    /**
     * @brief True once, on the update() where the connection came up.
     */
    bool justConnected();

    /**
     * @brief True once, on the update() where the connection dropped.
     */
    bool justDisconnected();

    /**
     * @brief True once per failed connection attempt: on the update() a
     * drop is first detected, and again for every WifiRetryIntervalMs spent
     * disconnected after that.
     */
    bool attemptFailed();

private:
    CredentialSource& credentialSource_;

    bool wasConnected_ = false;
    bool justConnected_ = false;
    bool justDisconnected_ = false;
    bool attemptFailed_ = false;
    unsigned long lastAttemptAt_ = 0;
};
