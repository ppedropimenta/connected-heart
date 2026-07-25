/**
* @file Pins.h
 * @brief Pin definitions for the ESP32-C3 SuperMini board.
 *
 * This file centralizes the hardware pin mapping of the project.
 * Application modules should use these constants instead of directly
 * referencing GPIO numbers.
 *
* Board:
 * - ESP32-C3 SuperMini
 * - ESP32-C3FN4
 * - 11 exposed GPIOs
 * - Onboard blue LED connected to GPIO8
 * - BOOT button connected to GPIO9
 */

#pragma once

#include <cstdint>

namespace Pins {
    using Pin = std::uint8_t;

    // Exposed GPIO pins
    constexpr Pin GPIO_0  = 0;
    constexpr Pin GPIO_1  = 1;
    constexpr Pin GPIO_2  = 2;
    constexpr Pin GPIO_3  = 3;
    constexpr Pin GPIO_4  = 4;
    constexpr Pin GPIO_5  = 5;
    constexpr Pin GPIO_6  = 6;
    constexpr Pin GPIO_7  = 7;
    constexpr Pin GPIO_8  = 8;
    constexpr Pin GPIO_9  = 9;
    constexpr Pin GPIO_10 = 10;

    // Onboard hardware
    /**
     * Blue onboard LED.
     *
     * The LED uses inverted logic:
     * - LOW  -> LED on
     * - HIGH -> LED off
     */
    constexpr Pin STATUS_LED = GPIO_8;

    /**
     * Onboard BOOT button.
     *
     * The button normally pulls GPIO9 low when pressed.
     * GPIO9 also influences the ESP32-C3 boot mode.
     */
    constexpr Pin BOOT_BUTTON = GPIO_9;

    // Analog-capable pins
    constexpr Pin ADC_0 = GPIO_0;
    constexpr Pin ADC_1 = GPIO_1;
    constexpr Pin ADC_2 = GPIO_2;
    constexpr Pin ADC_3 = GPIO_3;
}
