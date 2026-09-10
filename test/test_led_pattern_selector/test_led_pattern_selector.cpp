#include <unity.h>

#include "Led/LedPatternSelector.h"
#include "State/ConnectionState.h"
#include "State/DeviceState.h"

#include "fakes/FakeLed.h"

void setUp() {}
void tearDown() {}

namespace {
    struct Case {
        DeviceState deviceState;
        ConnectionState connectionState;
        LedPattern expected;
    };

    /**
     * @brief Every DeviceState x ConnectionState combination.
     *
     * Device state selects the pattern; ConnectionError only replaces a
     * pattern that would otherwise be Off, so Notification and
     * RevealingMessage keep their own rhythm no matter the connection.
     */
    constexpr Case kCases[] = {
        {DeviceState::Boot, ConnectionState::Connecting, LedPattern::Off},
        {DeviceState::Boot, ConnectionState::Connected, LedPattern::Off},
        {DeviceState::Boot, ConnectionState::Error, LedPattern::ConnectionError},

        {DeviceState::Setup, ConnectionState::Connecting, LedPattern::Setup},
        {DeviceState::Setup, ConnectionState::Connected, LedPattern::Setup},
        {DeviceState::Setup, ConnectionState::Error, LedPattern::Setup},

        {DeviceState::Waking, ConnectionState::Connecting, LedPattern::Connecting},
        {DeviceState::Waking, ConnectionState::Connected, LedPattern::Connecting},
        {DeviceState::Waking, ConnectionState::Error, LedPattern::Connecting},

        {DeviceState::WaitingForMessage, ConnectionState::Connecting, LedPattern::Off},
        {DeviceState::WaitingForMessage, ConnectionState::Connected, LedPattern::Off},
        {DeviceState::WaitingForMessage, ConnectionState::Error, LedPattern::ConnectionError},

        {DeviceState::Notification, ConnectionState::Connecting, LedPattern::NotificationPulse},
        {DeviceState::Notification, ConnectionState::Connected, LedPattern::NotificationPulse},
        {DeviceState::Notification, ConnectionState::Error, LedPattern::NotificationPulse},

        {DeviceState::RevealingMessage, ConnectionState::Connecting, LedPattern::RevealHeartbeat},
        {DeviceState::RevealingMessage, ConnectionState::Connected, LedPattern::RevealHeartbeat},
        {DeviceState::RevealingMessage, ConnectionState::Error, LedPattern::RevealHeartbeat},

        {DeviceState::DisplayingMessage, ConnectionState::Connecting, LedPattern::Off},
        {DeviceState::DisplayingMessage, ConnectionState::Connected, LedPattern::Off},
        {DeviceState::DisplayingMessage, ConnectionState::Error, LedPattern::ConnectionError},
    };

    void assertPattern(const LedPattern expected, const LedPattern actual) {
        TEST_ASSERT_EQUAL(static_cast<int>(expected), static_cast<int>(actual));
    }
}

void test_selects_the_pattern_for_every_state_combination() {
    FakeLed led;

    for (const Case& testCase : kCases) {
        led.show(LedPatternSelector::select(testCase.deviceState, testCase.connectionState));
        assertPattern(testCase.expected, led.lastPattern);
    }
}

void test_connection_error_never_interrupts_a_pending_notification() {
    FakeLed led;

    led.show(LedPatternSelector::select(DeviceState::Notification, ConnectionState::Error));

    assertPattern(LedPattern::NotificationPulse, led.lastPattern);
}

void test_connection_error_never_interrupts_a_reveal() {
    FakeLed led;

    led.show(LedPatternSelector::select(DeviceState::RevealingMessage, ConnectionState::Error));

    assertPattern(LedPattern::RevealHeartbeat, led.lastPattern);
}

int main(int argc, char** argv) {
    UNITY_BEGIN();

    RUN_TEST(test_selects_the_pattern_for_every_state_combination);
    RUN_TEST(test_connection_error_never_interrupts_a_pending_notification);
    RUN_TEST(test_connection_error_never_interrupts_a_reveal);

    return UNITY_END();
}
