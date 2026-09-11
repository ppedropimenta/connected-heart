/**
* @file CredentialSource.h
 * @brief Interface for supplying the WiFi credentials WifiManager connects
 * with.
 *
 * This is the seam behind which a compiled-in path
 * (CompiledCredentialSource, for bench work) and the captive portal's
 * provisioned path (ticket 11) are interchangeable. WifiManager depends on
 * this interface and never on a concrete source.
 */

#pragma once

#include "WifiCredentials.h"

class CredentialSource {
public:
    virtual ~CredentialSource() = default;

    virtual WifiCredentials credentials() const = 0;
};
