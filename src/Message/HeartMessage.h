#pragma once

#include <Arduino.h>

#include "MessageType.h"

/**
 * @brief Represents a message received by Connected Heart.
 *
 * For a custom message, text contains the message that should be displayed.
 *
 * For a predefined message, text remains empty and the corresponding
 * Portuguese text is resolved later by MessageCatalog.
 */
struct HeartMessage {
    MessageType type = MessageType::Custom;
    String text;

    /**
     * @brief Creates an empty custom message.
     */
    HeartMessage() = default;

    /**
     * @brief Creates a predefined message.
     *
     * The text remains empty because predefined messages are resolved
     * through MessageCatalog.
     *
     * @param messageType Type of the predefined message.
     */
    explicit HeartMessage(MessageType messageType)
        : type(messageType),
          text()
    {}

    /**
     * @brief Creates a custom message.
     *
     * @param customText Text that should be displayed.
     */
    explicit HeartMessage(const String& customText)
        : type(MessageType::Custom),
          text(customText)
    {}


};
