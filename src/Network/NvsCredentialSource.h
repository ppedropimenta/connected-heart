/**
* @file NvsCredentialSource.h
 * @brief Supplies WiFi credentials provisioned through the captive portal
 * (ProvisioningPortal) and persisted in ESP32 NVS.
 */

#pragma once

#include "CredentialSource.h"

/**
 * @brief Reads and writes WifiCredentials in NVS, under a namespace of
 * their own so they never collide with NvsMessageStorage's keys.
 */
class NvsCredentialSource : public CredentialSource {
public:
    WifiCredentials credentials() const override;

    /**
     * @brief Whether credentials have ever been successfully provisioned.
     *
     * Consulted at boot to decide between DeviceState::Waking and
     * DeviceState::Setup.
     */
    bool hasCredentials() const;

    /**
     * @brief Persists the given credentials, replacing whatever was saved
     * before.
     *
     * Called by ProvisioningPortal only once a candidate network has
     * actually been confirmed to connect, so storage never holds an
     * untested password.
     */
    void save(const WifiCredentials& credentials);

    /**
     * @brief Erases the stored credentials.
     *
     * The manual recovery path, per ADR-0004: called on a reset button
     * long press, alongside NvsPairingSource::clear() and
     * NvsMessageStorage::clear().
     */
    void clear();

private:
    /**
     * @brief Back the pointers returned by credentials(), whose caller
     * expects them to stay valid for the call's duration.
     */
    mutable char ssid_[33] = {};
    mutable char password_[64] = {};
};
