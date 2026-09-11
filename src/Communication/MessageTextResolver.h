/**
* @file MessageTextResolver.h
 * @brief Resolves a line of sender-typed text to a HeartMessage.
 *
 * Shared by SerialMessageSource and TelegramMessageSource so the
 * Feeling-command-or-Custom-text decision isn't duplicated between the
 * two MessageSource adapters that both sit on top of SerialLineParser.
 */

#pragma once

#include "Message/HeartMessage.h"

class MessageTextResolver {
public:
    /**
     * @brief Resolves text to a Predefined Message if it matches a
     * Feeling command SerialLineParser recognizes, or a Custom Message
     * carrying the text verbatim otherwise.
     *
     * @param text Null-terminated text. Must not be null.
     */
    static HeartMessage resolve(const char* text);
};
