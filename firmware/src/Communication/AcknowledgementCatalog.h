/**
* @file AcknowledgementCatalog.h
 * @brief Brazilian Portuguese wording for the Telegram bot's
 * acknowledgements and rejections, per ADR-0002 and ticket 10.
 *
 * Kept apart from TelegramMessageSource, which wraps a third-party library
 * and is not unit tested, so this wording is testable on the host.
 */

#pragma once

class AcknowledgementCatalog {
public:
    /**
     * @brief An accepted message that displaced nothing unrevealed.
     */
    static const char* delivered();

    /**
     * @brief An accepted message that displaced a Pending Message the
     * recipient had not yet revealed, per ADR-0001.
     */
    static const char* deliveredReplacingUnrevealed();

    /**
     * @brief A Custom Message rejected for being over
     * Config::MaximumMessageLength.
     */
    static const char* rejectedTooLong();

    /**
     * @brief An update with no usable text: an empty message, or
     * unsupported content such as a photo, sticker, or voice note.
     *
     * The two are not distinguishable from the Telegram update alone (see
     * TelegramMessageSource), so they share one reply worded to cover
     * both.
     */
    static const char* rejectedNotText();

    /**
     * @brief The correct pairing code was received, per ticket 12.
     *
     * A wrong code gets no reply at all: a stranger who finds the bot
     * before pairing should learn nothing from it.
     */
    static const char* paired();
};
