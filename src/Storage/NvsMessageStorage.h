/**
* @file NvsMessageStorage.h
 * @brief Persists the Current Message in ESP32 NVS.
 */

#pragma once

#include "MessageStorage.h"
#include "Message/HeartMessageSlot.h"

class NvsMessageStorage : public MessageStorage {
public:
    const HeartMessage* loadCurrentMessage() override;
    void saveCurrentMessage(const HeartMessage& message) override;

private:
    /**
     * @brief Backs the pointer returned by loadCurrentMessage(), whose
     * caller expects a stable object rather than a temporary.
     */
    HeartMessageSlot cached_;
};
