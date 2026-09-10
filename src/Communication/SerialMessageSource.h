#pragma once

#include <Arduino.h>

#include "Communication/MessageSource.h"
#include "Message/Feeling.h"
#include "Message/HeartMessage.h"

/**
 * @brief Turns lines typed into the serial monitor into HeartMessages.
 *
 * A line recognized by SerialLineParser as a Feeling command becomes a
 * Predefined Message; any other non-empty line becomes a Custom Message
 * carrying that line's text verbatim. Lines longer than the buffer are cut
 * off at its boundary; MessageManager is responsible for rejecting text
 * over the product's message length limit.
 *
 * This is a thin wrapper over Arduino's Serial and a bench-testing stand-in
 * for the eventual TelegramMessageSource; SerialLineParser carries the
 * testable parsing logic, so this class itself is not unit tested.
 */
class SerialMessageSource final : public MessageSource {
public:
    void begin() override;
    void update() override;

    bool hasMessage() const override;
    HeartMessage takeMessage() override;

private:
    static constexpr size_t LineBufferCapacity = HeartMessage::TextCapacity;

    char lineBuffer_[LineBufferCapacity] = {};
    size_t lineLength_ = 0;

    HeartMessage pending_ = HeartMessage(Feeling::Love);
    bool hasPending_ = false;

    void handleLine();
};
