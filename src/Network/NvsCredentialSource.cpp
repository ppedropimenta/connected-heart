#include "NvsCredentialSource.h"

#include <Preferences.h>

namespace {
    constexpr const char* Namespace = "wifi";
    constexpr const char* SsidKey = "ssid";
    constexpr const char* PasswordKey = "password";
}

WifiCredentials NvsCredentialSource::credentials() const {
    Preferences preferences;

    if (preferences.begin(Namespace, true)) {
        preferences.getString(SsidKey, ssid_, sizeof(ssid_));
        preferences.getString(PasswordKey, password_, sizeof(password_));
        preferences.end();
    }

    return {ssid_, password_};
}

bool NvsCredentialSource::hasCredentials() const {
    Preferences preferences;

    if (!preferences.begin(Namespace, true)) {
        return false;
    }

    const bool present = preferences.isKey(SsidKey);

    preferences.end();

    return present;
}

void NvsCredentialSource::save(const WifiCredentials& credentials) {
    Preferences preferences;

    if (!preferences.begin(Namespace, false)) {
        return;
    }

    preferences.putString(SsidKey, credentials.ssid);
    preferences.putString(PasswordKey, credentials.password);

    preferences.end();
}
