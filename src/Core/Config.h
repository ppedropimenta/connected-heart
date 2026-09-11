#pragma once

#include <cstddef>

namespace Config
{
    /**
     * How long the heart-alone reveal screen holds before the message is
     * drawn. Each full e-paper refresh already takes roughly two seconds,
     * so this is a deliberate pause on top of that anticipation, not the
     * whole of it. Provisional pending confirmation on the real panel;
     * see ticket 05.
     */
    constexpr unsigned long RevealDurationMs = 400;
    constexpr unsigned long TouchDebounceMs = 75;

    /**
     * Rhythm timings for the notification LED. LedPulsePeriodMs is the
     * Notification pulse referenced by ADR/spec as "the existing pulse
     * period"; the rest are provisional pending hardware tuning, per
     * ticket 06.
     */
    constexpr unsigned long LedPulsePeriodMs = 1600;
    constexpr unsigned long LedSetupBreathePeriodMs = 3200;
    constexpr unsigned long LedConnectingBlinkPeriodMs = 400;
    constexpr unsigned long LedRevealThumpPeriodMs = 900;
    constexpr unsigned long LedConnectionErrorBlinkPeriodMs = 3000;
    constexpr unsigned long LedConnectionErrorBlinkOnMs = 150;

    /**
     * Consecutive failed connection attempts before the heart gives up and
     * returns to Setup. Counted rather than timed, so a router that is slow
     * to answer is not treated the same as one that is gone.
     */
    constexpr unsigned int MaxConsecutiveConnectionFailures = 5;

    /**
     * How long WifiManager waits between reconnection attempts while
     * disconnected. Combined with MaxConsecutiveConnectionFailures this sets
     * how long the heart tries before giving up and returning to Setup;
     * provisional pending hardware tuning, per ticket 08.
     */
    constexpr unsigned long WifiRetryIntervalMs = 30000;

    /**
     * Counted in displayed characters after transliteration, not in bytes,
     * so accented Portuguese is not penalised. See MessageManager.
     */
    constexpr size_t MaximumMessageLength = 85;

    /**
     * How often TelegramMessageSource calls getUpdates(). Long enough to
     * stay well clear of Telegram's rate limits between messages, short
     * enough that a new message shows up promptly.
     */
    constexpr unsigned long TelegramPollIntervalMs = 3000;

    /**
     * Name of the access point Setup raises so an unprovisioned heart can
     * be configured from a phone. Fixed rather than derived from the chip,
     * since a recipient only ever configures one heart at a time. See
     * ProvisioningPortal.
     */
    constexpr const char* ProvisioningApSsid = "Connected Heart";

    /**
     * How long ProvisioningPortal waits for a submitted candidate network
     * to connect before treating it as a wrong password, discarding the
     * attempt, and returning the recipient to the configuration page.
     */
    constexpr unsigned long ProvisioningConnectTimeoutMs = 15000;
}
