#include "SerialLineParser.h"

#include <cstring>

bool SerialLineParser::tryParseFeeling(const char* line, Feeling& outFeeling) {
    if (std::strcmp(line, "/love") == 0) {
        outFeeling = Feeling::Love;
        return true;
    }

    if (std::strcmp(line, "/missyou") == 0) {
        outFeeling = Feeling::MissYou;
        return true;
    }

    if (std::strcmp(line, "/thinking") == 0) {
        outFeeling = Feeling::ThinkingOfYou;
        return true;
    }

    if (std::strcmp(line, "/goodmorning") == 0) {
        outFeeling = Feeling::GoodMorning;
        return true;
    }

    if (std::strcmp(line, "/goodnight") == 0) {
        outFeeling = Feeling::GoodNight;
        return true;
    }

    return false;
}
