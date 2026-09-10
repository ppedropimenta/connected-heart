#pragma once

#include <cstddef>

#include "HeartMessage.h"

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
     * @brief Holds at most one HeartMessage without heap allocation.
     *
     * HeartMessage has no default constructor, so it cannot sit in a plain
     * member to represent "no message" — absence is tracked by this slot
     * being empty, not by an in-band invalid message value. This would be
     * std::optional<HeartMessage>, but this project's toolchain builds at
     * -std=gnu++11, which predates it.
     */
    class Slot {
    public:
        Slot() = default;
        ~Slot();

        Slot(const Slot&) = delete;
        Slot& operator=(const Slot&) = delete;

        void set(const HeartMessage& message);
        void reset();

        [[nodiscard]] bool present() const { return present_; }
        [[nodiscard]] const HeartMessage* get() const;

    private:
        alignas(HeartMessage) unsigned char storage_[sizeof(HeartMessage)];
        bool present_ = false;
    };

    /**
     * @brief Checks whether a received message satisfies the model rules.
     */
    static bool isValid(const HeartMessage& message);

    Slot pending_;
    Slot current_;
};
