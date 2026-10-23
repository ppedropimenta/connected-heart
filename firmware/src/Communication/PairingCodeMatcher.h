/**
* @file PairingCodeMatcher.h
 * @brief Decides whether a message sent to an unpaired heart's bot claims
 * it (ADR-0004).
 *
 * Kept apart from TelegramMessageSource so the matching rule itself is
 * testable on the host; TelegramMessageSource is a thin Arduino/Telegram
 * wrapper around it and is not unit tested.
 */

#pragma once

class PairingCodeMatcher {
public:
    /**
     * @brief Whether text claims the heart for the given pairing code.
     *
     * Tolerates leading and trailing whitespace, so a stray space or
     * newline a Telegram client adds doesn't cost an otherwise-correct
     * code the match.
     *
     * @param code Null-terminated pairing code. Must not be null.
     * @param text Null-terminated message text. Must not be null.
     */
    static bool matches(const char* code, const char* text);
};
