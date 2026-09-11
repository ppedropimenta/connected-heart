#pragma once

#include <cstddef>

#include "HeartMessage.h"
#include "HeartMessageSlot.h"

/**
 * @brief Stores the currently displayed message and the next pending message.
 *
 * It only manages the lifecycle of received messages.
 *
 * A newly received message replaces any existing pending message,
 * but does not modify the currently displayed message.
 */
class MessageManager {
public:
    /**
     * @brief Stores a newly received message as pending.
     *
     * If another pending message already exists, it is replaced.
     * The currently displayed message is preserved.
     *
     * Custom messages with empty text, or text over the length limit,
     * are rejected.
     *
     * @param message Message to store.
     *
     * @return true if the message was accepted.
     * @return false if the message was invalid.
     */
    bool receive(const HeartMessage& message);

    /**
     * @brief Promotes the pending message to the current message.
     *
     * After a successful reveal, the pending message is removed.
     *
     * @return true if a pending message was revealed.
     * @return false if no pending message existed.
     */
    bool revealPending();

    /**
     * @brief Restores a message as the Current Message, e.g. loaded from
     * storage on boot.
     *
     * Unlike revealPending(), this does not touch the Pending Message: it
     * exists for restoring state that was never part of a reveal.
     */
    void restoreCurrent(const HeartMessage& message);

    bool hasPending() const;

    bool hasCurrent() const;

    /**
     * @brief Returns the pending message.
     *
     * @return Pointer to the pending message, or nullptr if none exists.
     */
    const HeartMessage* pending() const;

    /**
     * @brief Returns the currently displayed message.
     *
     * @return Pointer to the current message, or nullptr if none exists.
     */
    const HeartMessage* current() const;

    /**
     * @brief Removes both the pending and current messages.
     */
    void clear();

private:
    /**
     * @brief Checks whether a received message satisfies the model rules.
     */
    static bool isValid(const HeartMessage& message);

    HeartMessageSlot pending_;
    HeartMessageSlot current_;
};
