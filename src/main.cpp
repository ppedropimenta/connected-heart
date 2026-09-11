#include <Arduino.h>

#include "App/Application.h"
#include "Communication/CompiledPairingSource.h"
#include "Communication/TelegramMessageSource.h"
#include "Core/ArduinoClock.h"
#include "Core/Log.h"
#include "Display/EpaperDisplay.h"
#include "Display/EpaperDisplayRenderer.h"
#include "Led/NeoPixelLed.h"
#include "Network/CompiledCredentialSource.h"
#include "Network/NvsCredentialSource.h"
#include "Network/ProvisioningPortal.h"
#include "Network/Secrets.h"
#include "Network/WifiManager.h"
#include "Storage/NvsMessageStorage.h"
#include "Touch/GpioTouchSensor.h"

namespace {
    EpaperDisplay display;
    EpaperDisplayRenderer renderer(display);
    NeoPixelLed led;
    GpioTouchSensor touch;
    CompiledPairingSource pairingSource;
    TelegramMessageSource messageSource(TELEGRAM_BOT_TOKEN, pairingSource);
    NvsMessageStorage storage;
    ArduinoClock clock;

#ifdef USE_COMPILED_CREDENTIALS
    /*
     * Bench build: credentials are compiled in from Secrets.h and the
     * captive portal is skipped entirely, so nobody has to click through
     * it on every flash. See platformio.ini for how to enable this.
     */
    CompiledCredentialSource credentialSource;
#else
    NvsCredentialSource credentialSource;
    ProvisioningPortal provisioningPortal(credentialSource);
#endif

    WifiManager wifiManager(credentialSource);

    Application app(renderer, led, touch, messageSource, storage, clock);

    /**
     * @brief Polls WifiManager and translates its signals into Application
     * events. Shared by both the provisioned and captive-portal paths
     * through loop() below.
     */
    void handleWifiManagerEvents() {
        wifiManager.update();

        if (wifiManager.justConnected()) {
            app.notifyConnectionEstablished();
        }

        if (wifiManager.attemptFailed()) {
            app.notifyConnectionAttemptFailed();
        }
    }
}

void setup() {
    Serial.begin(115200);
    delay(500);

    LOG_INFO("Connected Heart starting");

    display.begin();
    led.begin();
    touch.begin();
    messageSource.begin();

#ifdef USE_COMPILED_CREDENTIALS
    const bool provisioned = true;
#else
    const bool provisioned = credentialSource.hasCredentials();
#endif

    if (provisioned) {
        wifiManager.begin();
    }

    app.begin(provisioned);

    LOG_INFO("Application initialized");
}

void loop() {
#ifndef USE_COMPILED_CREDENTIALS
    if (app.state() == DeviceState::Setup) {
        if (!provisioningPortal.active()) {
            provisioningPortal.begin();
        }

        provisioningPortal.update();

        if (provisioningPortal.justProvisioned()) {
            provisioningPortal.end();
            wifiManager.begin();
            app.notifyProvisioningFinished();
        }

        app.update();
        return;
    }

    if (provisioningPortal.active()) {
        provisioningPortal.end();
    }
#endif

    handleWifiManagerEvents();
    app.update();
}
