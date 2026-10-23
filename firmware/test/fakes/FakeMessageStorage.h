#pragma once

#include "Message/HeartMessageSlot.h"
#include "Storage/MessageStorage.h"

class FakeMessageStorage : public MessageStorage {
public:
    const HeartMessage* loadCurrentMessage() override {
        return slot_.get();
    }

    void saveCurrentMessage(const HeartMessage& message) override {
        slot_.set(message);
        ++saveCount;
    }

    /**
     * @brief Seeds the fake as if a message had been persisted before boot.
     */
    void seedCurrentMessage(const HeartMessage& message) {
        slot_.set(message);
    }

    int saveCount = 0;

private:
    HeartMessageSlot slot_;
};
