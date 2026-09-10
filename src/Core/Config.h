#pragma once

#include <cstddef>

namespace Config
{
    constexpr unsigned long RevealDurationMs = 1500;
    constexpr unsigned long TouchDebounceMs = 75;
    constexpr unsigned long LedPulsePeriodMs = 1600;

    /**
     * Consecutive failed connection attempts before the heart gives up and
     * returns to Setup. Counted rather than timed, so a router that is slow
     * to answer is not treated the same as one that is gone.
     */
    constexpr unsigned int MaxConsecutiveConnectionFailures = 5;

    /**
     * Counted in displayed characters after transliteration, not in bytes,
     * so accented Portuguese is not penalised. See MessageManager.
     */
    constexpr size_t MaximumMessageLength = 85;
}
