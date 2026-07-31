#pragma once

#include <Arduino.h>

#include "MessageType.h"

class MessageCatalog {
public:
    static String textFor(MessageType type);
};
