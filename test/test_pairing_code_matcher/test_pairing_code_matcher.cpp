#include <unity.h>

#include "Communication/PairingCodeMatcher.h"

void setUp() {}
void tearDown() {}

void test_exact_match_succeeds() {
    TEST_ASSERT_TRUE(PairingCodeMatcher::matches("482913", "482913"));
}

void test_wrong_code_is_rejected() {
    TEST_ASSERT_FALSE(PairingCodeMatcher::matches("482913", "111111"));
}

void test_empty_text_is_rejected() {
    TEST_ASSERT_FALSE(PairingCodeMatcher::matches("482913", ""));
}

void test_surrounding_whitespace_is_tolerated() {
    TEST_ASSERT_TRUE(PairingCodeMatcher::matches("482913", "  482913\n"));
}

void test_code_as_a_substring_of_longer_text_is_rejected() {
    TEST_ASSERT_FALSE(PairingCodeMatcher::matches("482913", "my code is 482913"));
}

void test_partial_prefix_is_rejected() {
    TEST_ASSERT_FALSE(PairingCodeMatcher::matches("482913", "48291"));
}

int main(int argc, char** argv) {
    UNITY_BEGIN();

    RUN_TEST(test_exact_match_succeeds);
    RUN_TEST(test_wrong_code_is_rejected);
    RUN_TEST(test_empty_text_is_rejected);
    RUN_TEST(test_surrounding_whitespace_is_tolerated);
    RUN_TEST(test_code_as_a_substring_of_longer_text_is_rejected);
    RUN_TEST(test_partial_prefix_is_rejected);

    return UNITY_END();
}
