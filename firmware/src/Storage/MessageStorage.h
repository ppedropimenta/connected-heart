/**
* @file MessageStorage.h
 * @brief Interface for persisting the Current Message across a power cycle.
 *
 * This is the non-volatile storage seam Application depends on. The
 * same NVS facility also holds the WiFi credentials and the paired
 * sender; this interface covers only the Current Message.
 *
 * The Pending Message is deliberately never persisted here: an unrevealed
 * message surviving a reboot would start the heart pulsing for something
 * that arrived days earlier, and the sender has already been told it
 * landed.
 */

#pragma once

#include "Message/HeartMessage.h"

class MessageStorage {
public:
    virtual ~MessageStorage() = default;

    /**
     * @brief Returns the persisted Current Message.
     *
     * @return Pointer to the restored message, or nullptr if none was ever
     * saved.
     */
    virtual const HeartMessage* loadCurrentMessage() = 0;

    /**
     * @brief Persists the given message as the Current Message, replacing
     * whatever was saved before.
     */
    virtual void saveCurrentMessage(const HeartMessage& message) = 0;
};
