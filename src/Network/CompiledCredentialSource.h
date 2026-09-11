/**
* @file CompiledCredentialSource.h
 * @brief Supplies WiFi credentials compiled into the firmware, for bench
 * work that skips the captive portal (see ProvisioningPortal).
 */

#pragma once

#include "CredentialSource.h"

/**
 * @brief Reads WifiCredentials from Secrets.h, which is excluded from
 * version control; see Secrets.example.h for the expected shape.
 */
class CompiledCredentialSource : public CredentialSource {
public:
    WifiCredentials credentials() const override;
};
