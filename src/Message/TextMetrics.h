#pragma once

#include <cstddef>

/**
 * @brief Counts text length the way it will appear on the display.
 */
namespace TextMetrics {
    /**
     * @brief Counts the displayed length of a UTF-8 string.
     *
     * Mirrors the decoding rule EpaperDisplay uses to transliterate
     * accented Portuguese characters: a 0xC3 lead byte followed by a
     * continuation byte becomes one displayed character, and any other
     * byte at or above 0x80 is unsupported by the display font and
     * contributes nothing. The two must be kept in sync by hand, since
     * this layer cannot depend on Arduino to share the implementation.
     *
     * @param text Null-terminated UTF-8 string.
     *
     * @return The number of characters text will occupy on the display.
     */
    [[nodiscard]] size_t displayedLength(const char* text);
}
