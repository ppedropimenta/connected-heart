//
// Created by caju2 on 25/07/2026.
//
#include <Arduino.h>

void setup() {
    Serial.begin(115200);

    delay(5000);

    Serial.println();
    Serial.println("SETUP EXECUTADO");
}

void loop() {
    Serial.printf("ESP32 alive pai pain: %lu ms\n", millis());
    Serial.flush();
    delay(1000);
}