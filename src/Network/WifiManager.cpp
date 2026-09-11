#include "WifiManager.h"

#include <WiFi.h>

#include "Core/Config.h"
#include "Core/Log.h"

WifiManager::WifiManager(CredentialSource& credentialSource)
    : credentialSource_(credentialSource)
{
}

void WifiManager::begin() {
    const WifiCredentials credentials = credentialSource_.credentials();

    LOG_INFO("Connecting to WiFi network %s", credentials.ssid);

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(credentials.ssid, credentials.password);

    lastAttemptAt_ = millis();
}

void WifiManager::update() {
    const bool connected = isConnected();

    if (connected) {
        if (!wasConnected_) {
            LOG_INFO("WiFi connected");
            justConnected_ = true;
        }

        wasConnected_ = true;
        return;
    }

    if (wasConnected_) {
        LOG_WARN("WiFi disconnected");

        justDisconnected_ = true;
        attemptFailed_ = true;
        wasConnected_ = false;
        lastAttemptAt_ = millis();

        WiFi.reconnect();
        return;
    }

    /*
     * Still disconnected, whether this is the first attempt from begin()
     * or a later retry. WiFi.setAutoReconnect() handles reconnection on its
     * own in most cases; the explicit reconnect() call here is a backstop
     * so an attempt is always retried even if the underlying stack gives up
     * silently.
     */
    if (millis() - lastAttemptAt_ >= Config::WifiRetryIntervalMs) {
        attemptFailed_ = true;
        lastAttemptAt_ = millis();

        WiFi.reconnect();
    }
}

bool WifiManager::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

bool WifiManager::justConnected() {
    const bool value = justConnected_;
    justConnected_ = false;
    return value;
}

bool WifiManager::justDisconnected() {
    const bool value = justDisconnected_;
    justDisconnected_ = false;
    return value;
}

bool WifiManager::attemptFailed() {
    const bool value = attemptFailed_;
    attemptFailed_ = false;
    return value;
}
