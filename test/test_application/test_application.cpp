#include <unity.h>

#include <string>

#include "App/Application.h"
#include "Core/Config.h"
#include "Message/Feeling.h"

#include "fakes/FakeClock.h"
#include "fakes/FakeDisplayRenderer.h"
#include "fakes/FakeLed.h"
#include "fakes/FakeMessageSource.h"
#include "fakes/FakeMessageStorage.h"
#include "fakes/FakeTouchSensor.h"

void setUp() {}
void tearDown() {}

namespace {
    /**
     * @brief Wires a fresh Application with fakes, already begin()-ed.
     */
    struct Fixture {
        FakeDisplayRenderer renderer;
        FakeLed led;
        FakeTouchSensor touch;
        FakeMessageSource messageSource;
        FakeMessageStorage storage;
        FakeClock clock;
        Application app{renderer, led, touch, messageSource, storage, clock};

        /**
         * @brief Begins the application already provisioned, landing in
         * Waking. Most tests care about behaviour past that point.
         */
        Fixture() {
            app.begin(true);
        }
    };

    /**
     * @brief Advances a provisioned Fixture from Waking to WaitingForMessage.
     */
    void connect(Fixture& fixture) {
        fixture.app.notifyConnectionEstablished();
    }

    /**
     * @brief Reports failed connection attempts until the heart gives up
     * and returns to Setup.
     */
    void exhaustConnectionAttempts(Fixture& fixture) {
        for (unsigned int i = 0; i < Config::MaxConsecutiveConnectionFailures; ++i) {
            fixture.app.notifyConnectionAttemptFailed();
        }
    }

    void assertScreen(
        const FakeDisplayRenderer::Screen expected,
        const FakeDisplayRenderer::Screen actual
    ) {
        TEST_ASSERT_EQUAL(
            static_cast<int>(expected),
            static_cast<int>(actual)
        );
    }

    void assertLedPattern(
        const LedPattern expected,
        const LedPattern actual
    ) {
        TEST_ASSERT_EQUAL(
            static_cast<int>(expected),
            static_cast<int>(actual)
        );
    }

    void assertConnectionState(
        const ConnectionState expected,
        const ConnectionState actual
    ) {
        TEST_ASSERT_EQUAL(
            static_cast<int>(expected),
            static_cast<int>(actual)
        );
    }

    void reveal(Fixture& fixture) {
        fixture.touch.press();
        fixture.app.update();

        fixture.clock.advance(Config::RevealDurationMs);
        fixture.app.update();
    }
}

void test_message_arriving_shows_notification() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage(Feeling::Love));
    fixture.app.update();

    assertScreen(FakeDisplayRenderer::Screen::Notification, fixture.renderer.lastScreen);
    assertLedPattern(LedPattern::NotificationPulse, fixture.led.lastPattern);
}

void test_message_while_pending_replaces_it() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage(Feeling::Love));
    fixture.app.update();

    fixture.messageSource.enqueue(HeartMessage("Boa noite"));
    fixture.app.update();

    assertScreen(FakeDisplayRenderer::Screen::Notification, fixture.renderer.lastScreen);

    fixture.touch.press();
    fixture.app.update();

    assertScreen(FakeDisplayRenderer::Screen::RevealHeart, fixture.renderer.lastScreen);
    assertLedPattern(LedPattern::RevealHeartbeat, fixture.led.lastPattern);

    fixture.clock.advance(Config::RevealDurationMs);
    fixture.app.update();

    assertScreen(FakeDisplayRenderer::Screen::Message, fixture.renderer.lastScreen);
    assertLedPattern(LedPattern::Off, fixture.led.lastPattern);
    TEST_ASSERT_EQUAL_STRING("Boa noite", fixture.renderer.lastMessageText.c_str());
}

void test_message_while_displayed_interrupts_it() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage("Primeira mensagem"));
    fixture.app.update();

    reveal(fixture);

    assertScreen(FakeDisplayRenderer::Screen::Message, fixture.renderer.lastScreen);
    TEST_ASSERT_EQUAL_STRING("Primeira mensagem", fixture.renderer.lastMessageText.c_str());

    fixture.messageSource.enqueue(HeartMessage("Segunda mensagem"));
    fixture.app.update();

    assertScreen(FakeDisplayRenderer::Screen::Notification, fixture.renderer.lastScreen);
}

void test_message_during_reveal_returns_to_notification_instead_of_displaying() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage("Primeira mensagem"));
    fixture.app.update();

    fixture.touch.press();
    fixture.app.update();

    assertScreen(FakeDisplayRenderer::Screen::RevealHeart, fixture.renderer.lastScreen);

    fixture.messageSource.enqueue(HeartMessage("Segunda mensagem"));
    fixture.app.update();

    assertScreen(FakeDisplayRenderer::Screen::RevealHeart, fixture.renderer.lastScreen);

    fixture.clock.advance(Config::RevealDurationMs);
    fixture.app.update();

    assertScreen(FakeDisplayRenderer::Screen::Notification, fixture.renderer.lastScreen);

    reveal(fixture);

    assertScreen(FakeDisplayRenderer::Screen::Message, fixture.renderer.lastScreen);
    TEST_ASSERT_EQUAL_STRING("Segunda mensagem", fixture.renderer.lastMessageText.c_str());
}

void test_reveal_does_not_finish_before_duration_elapses() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage("Primeira mensagem"));
    fixture.app.update();

    fixture.touch.press();
    fixture.app.update();

    assertScreen(FakeDisplayRenderer::Screen::RevealHeart, fixture.renderer.lastScreen);

    fixture.clock.advance(Config::RevealDurationMs - 1);
    fixture.app.update();

    assertScreen(FakeDisplayRenderer::Screen::RevealHeart, fixture.renderer.lastScreen);
    assertLedPattern(LedPattern::RevealHeartbeat, fixture.led.lastPattern);
}

void test_touch_during_reveal_does_nothing() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage("Primeira mensagem"));
    fixture.app.update();

    fixture.touch.press();
    fixture.app.update();

    assertScreen(FakeDisplayRenderer::Screen::RevealHeart, fixture.renderer.lastScreen);

    fixture.touch.press();
    fixture.app.update();

    assertScreen(FakeDisplayRenderer::Screen::RevealHeart, fixture.renderer.lastScreen);
}

void test_touch_while_message_displayed_does_nothing() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage("Primeira mensagem"));
    fixture.app.update();

    reveal(fixture);

    assertScreen(FakeDisplayRenderer::Screen::Message, fixture.renderer.lastScreen);

    fixture.touch.press();
    fixture.app.update();

    assertScreen(FakeDisplayRenderer::Screen::Message, fixture.renderer.lastScreen);
    TEST_ASSERT_EQUAL_STRING("Primeira mensagem", fixture.renderer.lastMessageText.c_str());
}

void test_predefined_message_resolves_to_catalog_wording() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage(Feeling::GoodMorning));
    fixture.app.update();

    reveal(fixture);

    assertScreen(FakeDisplayRenderer::Screen::Message, fixture.renderer.lastScreen);
    TEST_ASSERT_EQUAL_STRING("Bom dia, meu amor!", fixture.renderer.lastMessageText.c_str());
}

void test_revealing_a_message_persists_it_as_the_current_message() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage("Primeira mensagem"));
    fixture.app.update();

    reveal(fixture);

    TEST_ASSERT_EQUAL(1, fixture.storage.saveCount);

    const HeartMessage* persisted = fixture.storage.loadCurrentMessage();
    TEST_ASSERT_NOT_NULL(persisted);
    TEST_ASSERT_EQUAL_STRING("Primeira mensagem", persisted->text());
}

void test_receiving_a_pending_message_does_not_write_storage() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage(Feeling::Love));
    fixture.app.update();

    TEST_ASSERT_EQUAL(0, fixture.storage.saveCount);
    TEST_ASSERT_NULL(fixture.storage.loadCurrentMessage());
}

void test_storage_is_not_written_again_while_the_current_message_is_unchanged() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage("Primeira mensagem"));
    fixture.app.update();

    reveal(fixture);
    TEST_ASSERT_EQUAL(1, fixture.storage.saveCount);

    for (int i = 0; i < 5; ++i) {
        fixture.app.update();
    }

    TEST_ASSERT_EQUAL(1, fixture.storage.saveCount);
}

void test_restored_message_is_displayed_instead_of_the_waiting_screen() {
    FakeDisplayRenderer renderer;
    FakeLed led;
    FakeTouchSensor touch;
    FakeMessageSource messageSource;
    FakeMessageStorage storage;
    FakeClock clock;

    storage.seedCurrentMessage(HeartMessage("Ainda aqui"));

    Application app{renderer, led, touch, messageSource, storage, clock};

    app.begin(true);
    app.notifyConnectionEstablished();

    assertScreen(FakeDisplayRenderer::Screen::Message, renderer.lastScreen);
    TEST_ASSERT_EQUAL_STRING("Ainda aqui", renderer.lastMessageText.c_str());
}

void test_message_over_limit_is_rejected() {
    Fixture fixture;

    connect(fixture);
    assertScreen(FakeDisplayRenderer::Screen::WaitingForMessage, fixture.renderer.lastScreen);

    const std::string overLong(Config::MaximumMessageLength + 1, 'a');

    fixture.messageSource.enqueue(HeartMessage(overLong.c_str()));
    fixture.app.update();

    assertScreen(FakeDisplayRenderer::Screen::WaitingForMessage, fixture.renderer.lastScreen);
}

void test_message_of_multibyte_characters_exactly_one_over_limit_is_rejected() {
    Fixture fixture;

    connect(fixture);
    assertScreen(FakeDisplayRenderer::Screen::WaitingForMessage, fixture.renderer.lastScreen);

    /*
     * Config::MaximumMessageLength + 1 displayed characters, each a 2-byte
     * transliterated accented letter (see TextMetrics). Raw byte length is
     * (MaximumMessageLength + 1) * 2 = 172, one byte short of overflowing
     * HeartMessage::TextCapacity - 1 (170) by exactly the boundary that
     * used to let this slip past the length check: truncated to 170 bytes,
     * it decodes to exactly 85 characters, which is not over the limit.
     * The true, pre-truncation length is 86, which is.
     */
    std::string overLongMultibyte;
    for (size_t i = 0; i <= Config::MaximumMessageLength; ++i) {
        overLongMultibyte += "\xC3\xA3";
    }

    fixture.messageSource.enqueue(HeartMessage(overLongMultibyte.c_str()));
    fixture.app.update();

    assertScreen(FakeDisplayRenderer::Screen::WaitingForMessage, fixture.renderer.lastScreen);
}

void test_accepted_message_reports_outcome_without_displacement() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage(Feeling::Love));
    fixture.app.update();

    TEST_ASSERT_EQUAL(1, fixture.messageSource.outcomeReportCount);
    TEST_ASSERT_TRUE(fixture.messageSource.lastAccepted);
    TEST_ASSERT_FALSE(fixture.messageSource.lastWasPendingDisplaced);
}

void test_accepted_message_reports_outcome_with_displacement() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage(Feeling::Love));
    fixture.app.update();

    fixture.messageSource.enqueue(HeartMessage("Boa noite"));
    fixture.app.update();

    TEST_ASSERT_EQUAL(2, fixture.messageSource.outcomeReportCount);
    TEST_ASSERT_TRUE(fixture.messageSource.lastAccepted);
    TEST_ASSERT_TRUE(fixture.messageSource.lastWasPendingDisplaced);
}

void test_rejected_message_reports_outcome_as_not_accepted() {
    Fixture fixture;

    const std::string overLong(Config::MaximumMessageLength + 1, 'a');

    fixture.messageSource.enqueue(HeartMessage(overLong.c_str()));
    fixture.app.update();

    TEST_ASSERT_EQUAL(1, fixture.messageSource.outcomeReportCount);
    TEST_ASSERT_FALSE(fixture.messageSource.lastAccepted);
}

void test_touch_without_pending_message_does_nothing() {
    Fixture fixture;

    fixture.touch.press();
    fixture.app.update();

    assertScreen(FakeDisplayRenderer::Screen::Connecting, fixture.renderer.lastScreen);
}

void test_unprovisioned_boot_starts_in_setup() {
    FakeDisplayRenderer renderer;
    FakeLed led;
    FakeTouchSensor touch;
    FakeMessageSource messageSource;
    FakeMessageStorage storage;
    FakeClock clock;
    Application app{renderer, led, touch, messageSource, storage, clock};

    app.begin(false);

    assertScreen(FakeDisplayRenderer::Screen::Setup, renderer.lastScreen);
    assertLedPattern(LedPattern::Setup, led.lastPattern);
}

void test_provisioned_boot_starts_waking() {
    FakeDisplayRenderer renderer;
    FakeLed led;
    FakeTouchSensor touch;
    FakeMessageSource messageSource;
    FakeMessageStorage storage;
    FakeClock clock;
    Application app{renderer, led, touch, messageSource, storage, clock};

    app.begin(true);

    assertScreen(FakeDisplayRenderer::Screen::Connecting, renderer.lastScreen);
}

void test_provisioning_finished_moves_setup_to_waking() {
    FakeDisplayRenderer renderer;
    FakeLed led;
    FakeTouchSensor touch;
    FakeMessageSource messageSource;
    FakeMessageStorage storage;
    FakeClock clock;
    Application app{renderer, led, touch, messageSource, storage, clock};

    app.begin(false);
    assertScreen(FakeDisplayRenderer::Screen::Setup, renderer.lastScreen);
    assertLedPattern(LedPattern::Setup, led.lastPattern);

    app.notifyProvisioningFinished();

    assertScreen(FakeDisplayRenderer::Screen::Connecting, renderer.lastScreen);
    assertLedPattern(LedPattern::Connecting, led.lastPattern);
}

void test_connection_attempts_exhausted_from_waking_returns_to_setup() {
    Fixture fixture;

    assertScreen(FakeDisplayRenderer::Screen::Connecting, fixture.renderer.lastScreen);

    exhaustConnectionAttempts(fixture);

    assertScreen(FakeDisplayRenderer::Screen::Setup, fixture.renderer.lastScreen);
    assertLedPattern(LedPattern::Setup, fixture.led.lastPattern);
}

void test_connection_attempts_exhausted_from_waiting_for_message_returns_to_setup() {
    Fixture fixture;

    connect(fixture);
    assertScreen(FakeDisplayRenderer::Screen::WaitingForMessage, fixture.renderer.lastScreen);

    exhaustConnectionAttempts(fixture);

    assertScreen(FakeDisplayRenderer::Screen::Setup, fixture.renderer.lastScreen);
}

void test_connection_attempts_exhausted_from_notification_returns_to_setup() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage(Feeling::Love));
    fixture.app.update();
    assertScreen(FakeDisplayRenderer::Screen::Notification, fixture.renderer.lastScreen);

    exhaustConnectionAttempts(fixture);

    assertScreen(FakeDisplayRenderer::Screen::Setup, fixture.renderer.lastScreen);
}

void test_connection_attempts_exhausted_from_displaying_message_returns_to_setup() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage(Feeling::Love));
    fixture.app.update();

    reveal(fixture);
    assertScreen(FakeDisplayRenderer::Screen::Message, fixture.renderer.lastScreen);

    exhaustConnectionAttempts(fixture);

    assertScreen(FakeDisplayRenderer::Screen::Setup, fixture.renderer.lastScreen);
}

void test_connection_attempts_are_not_exhausted_before_the_configured_count() {
    Fixture fixture;

    for (unsigned int i = 0; i < Config::MaxConsecutiveConnectionFailures - 1; ++i) {
        fixture.app.notifyConnectionAttemptFailed();
    }

    assertScreen(FakeDisplayRenderer::Screen::Connecting, fixture.renderer.lastScreen);
}

void test_connection_error_shows_indicator_on_waiting_screen() {
    Fixture fixture;

    connect(fixture);
    assertScreen(FakeDisplayRenderer::Screen::WaitingForMessage, fixture.renderer.lastScreen);
    assertLedPattern(LedPattern::Off, fixture.led.lastPattern);

    fixture.app.notifyConnectionAttemptFailed();

    assertScreen(FakeDisplayRenderer::Screen::WaitingForMessage, fixture.renderer.lastScreen);
    assertConnectionState(ConnectionState::Error, fixture.renderer.lastConnectionState);
    assertLedPattern(LedPattern::ConnectionError, fixture.led.lastPattern);
}

void test_connection_error_shows_indicator_on_notification_screen_without_changing_led() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage(Feeling::Love));
    fixture.app.update();
    assertScreen(FakeDisplayRenderer::Screen::Notification, fixture.renderer.lastScreen);
    assertLedPattern(LedPattern::NotificationPulse, fixture.led.lastPattern);

    fixture.app.notifyConnectionAttemptFailed();

    assertScreen(FakeDisplayRenderer::Screen::Notification, fixture.renderer.lastScreen);
    assertConnectionState(ConnectionState::Error, fixture.renderer.lastConnectionState);

    /*
     * The more important thing (a message is waiting) wins the LED: the
     * display already carries the indicator, so the LED doesn't duplicate
     * it during a Notification. See spec Presentation / ADR-driven pattern
     * precedence.
     */
    assertLedPattern(LedPattern::NotificationPulse, fixture.led.lastPattern);
}

void test_connection_error_shows_indicator_during_reveal_without_changing_led() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage("Primeira mensagem"));
    fixture.app.update();

    fixture.touch.press();
    fixture.app.update();
    assertScreen(FakeDisplayRenderer::Screen::RevealHeart, fixture.renderer.lastScreen);

    fixture.app.notifyConnectionAttemptFailed();

    assertScreen(FakeDisplayRenderer::Screen::RevealHeart, fixture.renderer.lastScreen);
    assertConnectionState(ConnectionState::Error, fixture.renderer.lastConnectionState);
    assertLedPattern(LedPattern::RevealHeartbeat, fixture.led.lastPattern);
}

void test_connection_error_does_not_disturb_a_displayed_message() {
    Fixture fixture;

    fixture.messageSource.enqueue(HeartMessage("Primeira mensagem"));
    fixture.app.update();
    reveal(fixture);

    assertScreen(FakeDisplayRenderer::Screen::Message, fixture.renderer.lastScreen);
    TEST_ASSERT_EQUAL_STRING("Primeira mensagem", fixture.renderer.lastMessageText.c_str());
    assertLedPattern(LedPattern::Off, fixture.led.lastPattern);

    fixture.app.notifyConnectionAttemptFailed();

    assertScreen(FakeDisplayRenderer::Screen::Message, fixture.renderer.lastScreen);
    TEST_ASSERT_EQUAL_STRING("Primeira mensagem", fixture.renderer.lastMessageText.c_str());
    assertConnectionState(ConnectionState::Error, fixture.renderer.lastConnectionState);
    assertLedPattern(LedPattern::ConnectionError, fixture.led.lastPattern);
}

void test_reconnecting_clears_the_indicator() {
    Fixture fixture;

    connect(fixture);
    fixture.app.notifyConnectionAttemptFailed();
    assertScreen(FakeDisplayRenderer::Screen::WaitingForMessage, fixture.renderer.lastScreen);
    assertConnectionState(ConnectionState::Error, fixture.renderer.lastConnectionState);

    fixture.app.notifyConnectionEstablished();

    assertScreen(FakeDisplayRenderer::Screen::WaitingForMessage, fixture.renderer.lastScreen);
    assertConnectionState(ConnectionState::Connected, fixture.renderer.lastConnectionState);
    assertLedPattern(LedPattern::Off, fixture.led.lastPattern);
}

void test_connection_established_resets_the_failure_count() {
    Fixture fixture;

    for (unsigned int i = 0; i < Config::MaxConsecutiveConnectionFailures - 1; ++i) {
        fixture.app.notifyConnectionAttemptFailed();
    }

    connect(fixture);

    for (unsigned int i = 0; i < Config::MaxConsecutiveConnectionFailures - 1; ++i) {
        fixture.app.notifyConnectionAttemptFailed();
    }

    assertScreen(FakeDisplayRenderer::Screen::WaitingForMessage, fixture.renderer.lastScreen);
}

int main(int argc, char** argv) {
    UNITY_BEGIN();

    RUN_TEST(test_message_arriving_shows_notification);
    RUN_TEST(test_message_while_pending_replaces_it);
    RUN_TEST(test_message_while_displayed_interrupts_it);
    RUN_TEST(test_message_during_reveal_returns_to_notification_instead_of_displaying);
    RUN_TEST(test_reveal_does_not_finish_before_duration_elapses);
    RUN_TEST(test_touch_during_reveal_does_nothing);
    RUN_TEST(test_touch_while_message_displayed_does_nothing);
    RUN_TEST(test_predefined_message_resolves_to_catalog_wording);
    RUN_TEST(test_revealing_a_message_persists_it_as_the_current_message);
    RUN_TEST(test_receiving_a_pending_message_does_not_write_storage);
    RUN_TEST(test_storage_is_not_written_again_while_the_current_message_is_unchanged);
    RUN_TEST(test_restored_message_is_displayed_instead_of_the_waiting_screen);
    RUN_TEST(test_message_over_limit_is_rejected);
    RUN_TEST(test_message_of_multibyte_characters_exactly_one_over_limit_is_rejected);
    RUN_TEST(test_accepted_message_reports_outcome_without_displacement);
    RUN_TEST(test_accepted_message_reports_outcome_with_displacement);
    RUN_TEST(test_rejected_message_reports_outcome_as_not_accepted);
    RUN_TEST(test_touch_without_pending_message_does_nothing);
    RUN_TEST(test_unprovisioned_boot_starts_in_setup);
    RUN_TEST(test_provisioned_boot_starts_waking);
    RUN_TEST(test_provisioning_finished_moves_setup_to_waking);
    RUN_TEST(test_connection_attempts_exhausted_from_waking_returns_to_setup);
    RUN_TEST(test_connection_attempts_exhausted_from_waiting_for_message_returns_to_setup);
    RUN_TEST(test_connection_attempts_exhausted_from_notification_returns_to_setup);
    RUN_TEST(test_connection_attempts_exhausted_from_displaying_message_returns_to_setup);
    RUN_TEST(test_connection_attempts_are_not_exhausted_before_the_configured_count);
    RUN_TEST(test_connection_error_shows_indicator_on_waiting_screen);
    RUN_TEST(test_connection_error_shows_indicator_on_notification_screen_without_changing_led);
    RUN_TEST(test_connection_error_shows_indicator_during_reveal_without_changing_led);
    RUN_TEST(test_connection_error_does_not_disturb_a_displayed_message);
    RUN_TEST(test_reconnecting_clears_the_indicator);
    RUN_TEST(test_connection_established_resets_the_failure_count);

    return UNITY_END();
}
