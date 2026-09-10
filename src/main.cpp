#include <Arduino.h>

#include "App/Application.h"
#include "Communication/SerialMessageSource.h"
#include "Core/ArduinoClock.h"
#include "Core/Log.h"
#include "Display/EpaperDisplay.h"
#include "Display/EpaperDisplayRenderer.h"
#include "Led/NeoPixelLed.h"
#include "Touch/GpioTouchSensor.h"

namespace {
    EpaperDisplay display;
    EpaperDisplayRenderer renderer(display);
    NeoPixelLed led;
    GpioTouchSensor touch;
    SerialMessageSource messageSource;
    ArduinoClock clock;

    Application app(renderer, led, touch, messageSource, clock);
}

void setup() {
    Serial.begin(115200);
    delay(500);

    LOG_INFO("Connected Heart starting");

    display.begin();
    led.begin();
    touch.begin();
    messageSource.begin();

    /*
     * WifiManager does not exist yet (ticket 08), so there is no real
     * network to wait for. The heart is treated as already provisioned and
     * connected, landing straight in WaitingForMessage, so the receive
     * path built here can be exercised end to end over Serial.
     */
    app.begin(true);
    app.notifyConnectionEstablished();

    LOG_INFO("Application initialized");
}

void loop() {
    app.update();
}
