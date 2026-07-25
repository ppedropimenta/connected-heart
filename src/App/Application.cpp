#include "Application.h"

#include <Arduino.h>

#include "Core/Log.h"

void Application::begin() {
    Serial.begin(115200);

    delay(1000);

    LOG_INFO("Application starting");

    for (Module* module : modules_) {
        module->begin();
    }

    LOG_INFO("Application initialized");
}

void Application::update() {
    for (Module* module : modules_) {
        module->update();
    }
}
