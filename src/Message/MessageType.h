#pragma once

/**
 * @brief Defines the message categories supported by Connected Heart.
 *
 * Custom messages use the text stored in HeartMessage.
 * Predefined messages use text provided by MessageCatalog.
 */
enum class MessageType {
    Custom,
    Love,
    MissYou,
    ThinkingOfYou,
    GoodMorning,
    GoodNight
};
