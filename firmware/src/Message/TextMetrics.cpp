#include "TextMetrics.h"

size_t TextMetrics::displayedLength(const char* text) {
    size_t length = 0;

    for (size_t i = 0; text[i] != '\0'; ++i) {
        const auto current = static_cast<unsigned char>(text[i]);

        if (current == 0xC3 && text[i + 1] != '\0') {
            ++i;
            ++length;
            continue;
        }

        if (current < 0x80) {
            ++length;
        }
    }

    return length;
}
