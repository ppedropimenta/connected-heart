#pragma once

#include <Arduino.h>

namespace Config
{
    constexpr unsigned long RevealDurationMs = 1500;
    constexpr unsigned long TouchDebounceMs = 75;
    constexpr unsigned long LedPulsePeriodMs = 1600;

    constexpr size_t MaximumMessageLength = 180;
}
