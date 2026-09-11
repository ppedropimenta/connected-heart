#include "CompiledCredentialSource.h"

#include "Secrets.h"

WifiCredentials CompiledCredentialSource::credentials() const {
    return {WIFI_SSID, WIFI_PASSWORD};
}
