#include <Arduino.h>

#include <Core/Log.h>

/*
 * Application now takes its collaborators through its constructor (see
 * App/Application.h), so it cannot be default-constructed here. Wiring the
 * real hardware modules together is the entry point's job again once they
 * exist.
 */

void setup() {
    Serial.begin(115200);
    delay(500);

    LOG_INFO("Connected Heart starting");

    pinMode(21, INPUT);

    LOG_INFO("Application initialized");
}

void loop() {
    Serial.println(digitalRead(21));

    delay(50);
}

