#include <unity.h>

#include "Communication/MessageTextResolver.h"

void setUp() {}
void tearDown() {}

void test_feeling_command_resolves_to_a_predefined_message() {
    const HeartMessage message = MessageTextResolver::resolve("/love");

    TEST_ASSERT_EQUAL(static_cast<int>(MessageKind::Predefined), static_cast<int>(message.kind()));
    TEST_ASSERT_EQUAL(static_cast<int>(Feeling::Love), static_cast<int>(message.feeling()));
}

void test_plain_text_resolves_to_a_custom_message_with_that_text() {
    const HeartMessage message = MessageTextResolver::resolve("Bom dia, meu amor!");

    TEST_ASSERT_EQUAL(static_cast<int>(MessageKind::Custom), static_cast<int>(message.kind()));
    TEST_ASSERT_EQUAL_STRING("Bom dia, meu amor!", message.text());
}

void test_unknown_slash_command_resolves_to_a_custom_message_verbatim() {
    const HeartMessage message = MessageTextResolver::resolve("/unknown");

    TEST_ASSERT_EQUAL(static_cast<int>(MessageKind::Custom), static_cast<int>(message.kind()));
    TEST_ASSERT_EQUAL_STRING("/unknown", message.text());
}

int main(int argc, char** argv) {
    UNITY_BEGIN();

    RUN_TEST(test_feeling_command_resolves_to_a_predefined_message);
    RUN_TEST(test_plain_text_resolves_to_a_custom_message_with_that_text);
    RUN_TEST(test_unknown_slash_command_resolves_to_a_custom_message_verbatim);

    return UNITY_END();
}
