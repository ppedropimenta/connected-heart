#include "PairingCodeMatcher.h"

#include <cctype>
#include <cstring>

namespace {
    bool isSpace(const char c) {
        return std::isspace(static_cast<unsigned char>(c)) != 0;
    }
}

bool PairingCodeMatcher::matches(const char* code, const char* text) {
    while (*text != '\0' && isSpace(*text)) {
        ++text;
    }

    size_t length = std::strlen(text);

    while (length > 0 && isSpace(text[length - 1])) {
        --length;
    }

    const size_t codeLength = std::strlen(code);

    if (length == 0 || length != codeLength) {
        return false;
    }

    return std::strncmp(text, code, codeLength) == 0;
}
