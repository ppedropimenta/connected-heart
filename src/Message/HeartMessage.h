#pragma once

#include <cstddef>
#include <cstring>

#include "Feeling.h"
#include "MessageKind.h"
#include "Core/Config.h"

/**
 * @brief A single piece of affection sent to a Connected Heart.
 *
 * A HeartMessage carries either text (a Custom Message) or a Feeling
 * (a Predefined Message), never both and never neither. There is no way
 * to construct one in an incomplete state.
 */
struct HeartMessage {
    /**
     * Byte capacity of the text buffer. Sized for the worst case of
     * MaximumMessageLength *displayed* characters that are all
     * transliterated accented letters, which occupy two UTF-8 bytes apiece
     * (see TextMetrics) — not simply MaximumMessageLength + 1 bytes, which
     * would let this constructor silently truncate a message that is
     * within the displayed-character limit before MessageManager ever
     * gets to measure it.
     */
    static constexpr size_t TextCapacity = (Config::MaximumMessageLength * 2) + 1;

    /**
     * @brief Creates a Predefined Message carrying the given Feeling.
     */
    explicit HeartMessage(const Feeling feeling)
        : kind_(MessageKind::Predefined)
    {
        payload_.feeling = feeling;
    }

    /**
     * @brief Creates a Custom Message carrying its own text.
     *
     * @param text Null-terminated text to display. Must not be null.
     * Text beyond TextCapacity - 1 bytes is truncated.
     */
    explicit HeartMessage(const char* text)
        : kind_(MessageKind::Custom)
    {
        std::strncpy(payload_.text, text, TextCapacity - 1);
        payload_.text[TextCapacity - 1] = '\0';
    }

    [[nodiscard]] MessageKind kind() const { return kind_; }

    /**
     * @brief The Feeling this message carries.
     *
     * Only meaningful when kind() is MessageKind::Predefined.
     */
    [[nodiscard]] Feeling feeling() const { return payload_.feeling; }

    /**
     * @brief The text this message carries.
     *
     * Only meaningful when kind() is MessageKind::Custom.
     */
    [[nodiscard]] const char* text() const { return payload_.text; }

private:
    /**
     * @brief Holds either a Feeling or text, never both at once.
     *
     * Both members are trivial types, so a plain union needs no manual
     * lifetime handling here: whichever constructor runs writes exactly
     * one member, and callers are expected to read it back through the
     * matching accessor for kind().
     */
    union Payload {
        Feeling feeling;
        char text[TextCapacity];
    };

    MessageKind kind_;
    Payload payload_;
};
