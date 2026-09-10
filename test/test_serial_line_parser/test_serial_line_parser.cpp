#include <unity.h>

#include "Communication/SerialLineParser.h"

void setUp() {}
void tearDown() {}

namespace {
    void assertParsesFeeling(const char* line, const Feeling expected) {
        Feeling actual = Feeling::Love;

        TEST_ASSERT_TRUE(SerialLineParser::tryParseFeeling(line, actual));
        TEST_ASSERT_EQUAL(static_cast<int>(expected), static_cast<int>(actual));
    }
}

void test_love_command_parses_to_love_feeling() {
    assertParsesFeeling("/love", Feeling::Love);
}

void test_missyou_command_parses_to_missyou_feeling() {
    assertParsesFeeling("/missyou", Feeling::MissYou);
}

void test_thinking_command_parses_to_thinkingofyou_feeling() {
    assertParsesFeeling("/thinking", Feeling::ThinkingOfYou);
}

void test_goodmorning_command_parses_to_goodmorning_feeling() {
    assertParsesFeeling("/goodmorning", Feeling::GoodMorning);
}

void test_goodnight_command_parses_to_goodnight_feeling() {
    assertParsesFeeling("/goodnight", Feeling::GoodNight);
}

void test_plain_text_is_not_a_feeling_command() {
    Feeling outFeeling = Feeling::Love;

    TEST_ASSERT_FALSE(SerialLineParser::tryParseFeeling("Bom dia, meu amor!", outFeeling));
}

void test_empty_line_is_not_a_feeling_command() {
    Feeling outFeeling = Feeling::Love;

    TEST_ASSERT_FALSE(SerialLineParser::tryParseFeeling("", outFeeling));
}

void test_unknown_slash_command_is_not_a_feeling_command() {
    Feeling outFeeling = Feeling::Love;

    TEST_ASSERT_FALSE(SerialLineParser::tryParseFeeling("/unknown", outFeeling));
}

int main(int argc, char** argv) {
    UNITY_BEGIN();

    RUN_TEST(test_love_command_parses_to_love_feeling);
    RUN_TEST(test_missyou_command_parses_to_missyou_feeling);
    RUN_TEST(test_thinking_command_parses_to_thinkingofyou_feeling);
    RUN_TEST(test_goodmorning_command_parses_to_goodmorning_feeling);
    RUN_TEST(test_goodnight_command_parses_to_goodnight_feeling);
    RUN_TEST(test_plain_text_is_not_a_feeling_command);
    RUN_TEST(test_empty_line_is_not_a_feeling_command);
    RUN_TEST(test_unknown_slash_command_is_not_a_feeling_command);

    return UNITY_END();
}
