#include <Arduino.h>

#include <App/Application.h>
#include <Core/Log.h>

namespace {
    Application app;
}

void setup() {
    Serial.begin(115200);
    delay(500);

    LOG_INFO("Connected Heart starting");

    app.begin();

    LOG_INFO("Application initialized");
}

void loop() {
    app.update();
}

