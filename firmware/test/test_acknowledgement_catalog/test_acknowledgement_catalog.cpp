#include <unity.h>

#include <cstring>

#include "Communication/AcknowledgementCatalog.h"

void setUp() {}
void tearDown() {}

void test_wordings_are_in_portuguese_and_non_empty() {
    TEST_ASSERT_TRUE(std::strlen(AcknowledgementCatalog::delivered()) > 0);
    TEST_ASSERT_TRUE(std::strlen(AcknowledgementCatalog::deliveredReplacingUnrevealed()) > 0);
    TEST_ASSERT_TRUE(std::strlen(AcknowledgementCatalog::rejectedTooLong()) > 0);
    TEST_ASSERT_TRUE(std::strlen(AcknowledgementCatalog::rejectedNotText()) > 0);
    TEST_ASSERT_TRUE(std::strlen(AcknowledgementCatalog::paired()) > 0);
}

void test_every_wording_is_distinct() {
    const char* wordings[] = {
        AcknowledgementCatalog::delivered(),
        AcknowledgementCatalog::deliveredReplacingUnrevealed(),
        AcknowledgementCatalog::rejectedTooLong(),
        AcknowledgementCatalog::rejectedNotText(),
        AcknowledgementCatalog::paired(),
    };

    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = i + 1; j < 5; ++j) {
            TEST_ASSERT_NOT_EQUAL(0, std::strcmp(wordings[i], wordings[j]));
        }
    }
}

void test_displacement_wording_mentions_the_earlier_message_was_lost() {
    const char* text = AcknowledgementCatalog::deliveredReplacingUnrevealed();

    TEST_ASSERT_NOT_NULL(std::strstr(text, "anterior"));
    TEST_ASSERT_NOT_NULL(std::strstr(text, "substitu"));
}

void test_rejected_not_text_mentions_both_empty_and_unsupported_content() {
    const char* text = AcknowledgementCatalog::rejectedNotText();

    TEST_ASSERT_NOT_NULL(std::strstr(text, "vazia"));
    TEST_ASSERT_NOT_NULL(std::strstr(text, "texto"));
}

int main(int argc, char** argv) {
    UNITY_BEGIN();

    RUN_TEST(test_wordings_are_in_portuguese_and_non_empty);
    RUN_TEST(test_every_wording_is_distinct);
    RUN_TEST(test_displacement_wording_mentions_the_earlier_message_was_lost);
    RUN_TEST(test_rejected_not_text_mentions_both_empty_and_unsupported_content);

    return UNITY_END();
}
