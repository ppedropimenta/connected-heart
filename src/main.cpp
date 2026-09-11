#include <Arduino.h>

#include "App/Application.h"
#include "Communication/CompiledPairingSource.h"
#include "Communication/NvsPairingSource.h"
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
    NvsMessageStorage storage;
    ArduinoClock clock;

#ifdef USE_COMPILED_CREDENTIALS
    /*
     * Bench build: credentials and the paired chat identifier are compiled
     * in from Secrets.h, and the captive portal and pairing-by-code flow
     * are both skipped entirely, so nobody has to click through them on
     * every flash. See platformio.ini for how to enable this.
     */
    CompiledCredentialSource credentialSource;
    CompiledPairingSource pairingSource;
#else
    NvsCredentialSource credentialSource;
    NvsPairingSource pairingSource;
    ProvisioningPortal provisioningPortal(credentialSource, pairingSource);
#endif

    TelegramMessageSource messageSource(TELEGRAM_BOT_TOKEN, pairingSource);
    WifiManager wifiManager(credentialSource);

    Application app(renderer, led, touch, messageSource, storage, clock, pairingSource);

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
    pairingSource.ensureCode();

    const bool provisioned = credentialSource.hasCredentials() && pairingSource.isPaired();
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

        /*
         * app.update() runs Telegram polling regardless of DeviceState, so
         * a pairing message sent while Setup is still up (the portal's own
         * STA connection is already live once WiFi is confirmed, even
         * before wifiManager.begin() below) can complete on this very
         * tick. justProvisioned() is checked after update() so that can
         * happen: it only turns true once WiFi has connected AND pairing
         * has completed, however many update()s apart those land.
         */
        app.update();

        if (provisioningPortal.justProvisioned()) {
            provisioningPortal.end();
            wifiManager.begin();
            app.notifyProvisioningFinished();
        }

        return;
    }

    if (provisioningPortal.active()) {
        provisioningPortal.end();
    }
#endif

    handleWifiManagerEvents();
    app.update();
}
