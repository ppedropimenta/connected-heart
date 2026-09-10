#include <Arduino.h>

#include "App/Application.h"
#include "Communication/SerialMessageSource.h"
#include "Core/ArduinoClock.h"
#include "Core/Log.h"
#include "Display/EpaperDisplay.h"
#include "Display/EpaperDisplayRenderer.h"
#include "Led/Led.h"
#include "Touch/TouchSensor.h"

namespace {
    /**
     * @brief Led with no hardware behind it yet.
     *
     * Application requires a concrete Led to be constructed; the real
     * NeoPixel implementation arrives in ticket 06.
     */
    class NullLed final : public Led {
    public:
        void begin() override {}
        void update() override {}
        void show(LedPattern) override {}
    };

    /**
     * @brief TouchSensor that never reports a press.
     *
     * Application requires a concrete TouchSensor to be constructed; the
     * real debounced GPIO21 implementation arrives in ticket 05.
     */
    class NullTouchSensor final : public TouchSensor {
    public:
        void begin() override {}
        void update() override {}
        bool wasPressed() override { return false; }
    };

    EpaperDisplay display;
    EpaperDisplayRenderer renderer(display);
    NullLed led;
    NullTouchSensor touch;
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
