#pragma once

#include <cstddef>

namespace Config
{
    constexpr unsigned long RevealDurationMs = 1500;
    constexpr unsigned long TouchDebounceMs = 75;
    constexpr unsigned long LedPulsePeriodMs = 1600;

    /**
     * Counted in displayed characters after transliteration, not in bytes,
     * so accented Portuguese is not penalised. See MessageManager.
     */
    constexpr size_t MaximumMessageLength = 85;
}
