#include "MessageTextResolver.h"

#include "Message/Feeling.h"
#include "SerialLineParser.h"

HeartMessage MessageTextResolver::resolve(const char* text) {
    Feeling feeling;

    if (SerialLineParser::tryParseFeeling(text, feeling)) {
        return HeartMessage(feeling);
    }

    return HeartMessage(text);
}
