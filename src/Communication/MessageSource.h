#pragma once

#include "Core/Module.h"
#include "Message/HeartMessage.h"

class MessageSource : public Module {
public:
    virtual bool hasMessage() const = 0;
    virtual HeartMessage takeMessage() = 0;

    /**
     * @brief Reports what became of the last message takeMessage() handed
     * over, once Application has attempted to store it as Pending.
     *
     * Default no-op: only a source able to reply to the sender (see
     * TelegramMessageSource) needs this.
     *
     * @param accepted Whether MessageManager accepted the message.
     * @param wasPendingDisplaced Only meaningful when accepted is true:
     * whether an existing Pending Message that had never been revealed
     * was discarded to make room for this one, per ADR-0001.
     */
    virtual void reportOutcome(bool accepted, bool wasPendingDisplaced) {}
};
