#pragma once

#include "Feeling.h"
#include "HeartMessage.h"

class MessageCatalog {
public:
    static const char* textFor(Feeling feeling);

    /**
     * @brief Resolves a message to its final display text.
     *
     * A Predefined Message resolves to its catalog wording; a Custom
     * Message keeps its own text.
     */
    static const char* resolve(const HeartMessage& message);
};
