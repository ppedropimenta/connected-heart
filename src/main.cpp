#include <Arduino.h>

#include "App/Application.h"
#include "Communication/SerialMessageSource.h"
#include "Core/ArduinoClock.h"
#include "Core/Log.h"
#include "Display/EpaperDisplay.h"
#include "Display/EpaperDisplayRenderer.h"
#include "Led/NeoPixelLed.h"
#include "Network/CompiledCredentialSource.h"
#include "Network/WifiManager.h"
#include "Storage/NvsMessageStorage.h"
#include "Touch/GpioTouchSensor.h"

namespace {
    EpaperDisplay display;
    EpaperDisplayRenderer renderer(display);
    NeoPixelLed led;
    GpioTouchSensor touch;
    SerialMessageSource messageSource;
    NvsMessageStorage storage;
    ArduinoClock clock;
    CompiledCredentialSource credentialSource;
    WifiManager wifiManager(credentialSource);

    Application app(renderer, led, touch, messageSource, storage, clock);
}

void setup() {
    Serial.begin(115200);
    delay(500);

    LOG_INFO("Connected Heart starting");

    display.begin();
    led.begin();
    touch.begin();
    messageSource.begin();
    wifiManager.begin();

    /*
     * The captive portal doesn't exist yet (ticket 11), so credentials are
     * always compiled in (CompiledCredentialSource) and the heart is
     * treated as already provisioned. Whether it can actually reach that
     * network is reported for real by wifiManager, below.
     */
    app.begin(true);

    LOG_INFO("Application initialized");
}

void loop() {
    wifiManager.update();

    if (wifiManager.justConnected()) {
        app.notifyConnectionEstablished();
    }

    if (wifiManager.attemptFailed()) {
        app.notifyConnectionAttemptFailed();
    }

    app.update();
}
