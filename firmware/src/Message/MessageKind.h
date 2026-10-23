#pragma once

/**
 * @brief Distinguishes a Custom Message from a Predefined Message.
 *
 * A HeartMessage carries exactly one of the two: a Custom Message carries
 * its own text, a Predefined Message carries a Feeling resolved to text
 * later by MessageCatalog.
 */
enum class MessageKind {
    Custom,
    Predefined
};
