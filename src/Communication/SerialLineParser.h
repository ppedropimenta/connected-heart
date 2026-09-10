#pragma once

#include "Message/Feeling.h"

/**
 * @brief Parses a line typed into the serial monitor.
 *
 * Kept apart from SerialMessageSource so the parsing rule itself is
 * testable on the host; SerialMessageSource is a thin Arduino Serial
 * wrapper around it and is not unit tested.
 */
class SerialLineParser {
public:
    /**
     * @brief Attempts to read a line as a Feeling command.
     *
     * Recognizes "/love", "/missyou", "/thinking", "/goodmorning", and
     * "/goodnight". Any other line, including plain text and an empty
     * line, is not a Feeling command and should be treated as the text of
     * a Custom Message instead.
     *
     * @param line Null-terminated line, without its trailing newline.
     * @param outFeeling Set only when the line matches a command.
     *
     * @return true if line matched a Feeling command.
     */
    static bool tryParseFeeling(const char* line, Feeling& outFeeling);
};
