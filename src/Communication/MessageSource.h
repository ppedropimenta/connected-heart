#pragma once

#include "Core/Module.h"
#include "Message/HeartMessage.h"

class MessageSource : public Module {
public:
    virtual bool hasMessage() const = 0;
    virtual HeartMessage takeMessage() = 0;
};
