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
 * - 13 exposed GPIOs, confirmed on the bench: the header silkscreen
 *   comment this file used to carry claimed 11 while listing 13, and
 *   GPIO5 and GPIO20 (both now wired below) have since been proven out.
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
    constexpr Pin GPIO_20 = 20;
    constexpr Pin GPIO_21 = 21;

    namespace Display {
        constexpr Pin BUSY = GPIO_3;
        constexpr Pin SCK  = GPIO_4;
        constexpr Pin DC   = GPIO_1;
        constexpr Pin MOSI = GPIO_6;
        constexpr Pin CS   = GPIO_7;
        constexpr Pin RST  = GPIO_10;
    }

    namespace NotificationLed {
        constexpr Pin LED_DATA = GPIO_5;
    }

    namespace Touch {
        constexpr Pin TOUCH = GPIO_0;
    }

    namespace ResetButton {
        /**
         * Not GPIO2, GPIO8, or GPIO9: all three are strapping pins, and a
         * user-facing button on one of them drops the chip into download
         * mode if held during power-on (ADR-0004). Not GPIO5 either: the
         * notification LED's data line already lives there on this board's
         * actual bench wiring (see NotificationLed above).
         */
        constexpr Pin BUTTON = GPIO_20;
    }

    /**
     * GPIO2 is the only pin left genuinely free on this board; it is
     * RTC-capable. ADR-0005 also names GPIO0 as a spare, but GPIO0 is
     * committed to Touch above on this board's actual bench wiring.
     *
     * No Battery namespace/pin reservation exists here: per ADR-0005 the
     * heart is always powered, and the firmware knows nothing about a
     * battery even if one is fitted as a hardware-only backup (ticket 13).
     */

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
