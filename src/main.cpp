#include <Arduino.h>

#include <App/Application.h>
#include <Core/Log.h>

namespace {
    Application app;
}

void setup() {
    LOG_INFO("System started");
    LOG_DEBUG("Free heap: %u bytes", ESP.getFreeHeap());

    app.begin();
}

void loop() {
    app.update();

    Serial.println("ESP32 alive");
    Serial.flush();

    delay(1000);
}