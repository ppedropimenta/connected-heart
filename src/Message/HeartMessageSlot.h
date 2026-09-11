#pragma once

#include "HeartMessage.h"

/**
 * @brief Holds at most one HeartMessage without heap allocation.
 *
 * HeartMessage has no default constructor, so it cannot sit in a plain
 * member to represent "no message" — absence is tracked by this slot
 * being empty, not by an in-band invalid message value. This would be
 * std::optional<HeartMessage>, but this project's toolchain builds at
 * -std=gnu++11, which predates it.
 */
class HeartMessageSlot {
public:
    HeartMessageSlot() = default;
    ~HeartMessageSlot();

    HeartMessageSlot(const HeartMessageSlot&) = delete;
    HeartMessageSlot& operator=(const HeartMessageSlot&) = delete;

    void set(const HeartMessage& message);
    void reset();

    [[nodiscard]] bool present() const { return present_; }
    [[nodiscard]] const HeartMessage* get() const;

private:
    alignas(HeartMessage) unsigned char storage_[sizeof(HeartMessage)];
    bool present_ = false;
};
