#include "MessageManager.h"

bool MessageManager::receive(const HeartMessage& message) {
    if (!isValid(message)) {
        return false;
    }

    if (message.type == MessageType::Custom) {
        pending_ = message;
    } else {
        pending_ = HeartMessage(message.type);
    }

    hasPending_ = true;

    return true;
}

bool MessageManager::revealPending() {
    if (!hasPending_) {
        return false;
    }

    current_ = pending_;
    hasCurrent_ = true;

    pending_ = HeartMessage();
    hasPending_ = false;

    return true;
}

bool MessageManager::hasPending() const {
    return hasPending_;
}

bool MessageManager::hasCurrent() const {
    return hasCurrent_;
}

const HeartMessage* MessageManager::pending() const {
    if (!hasPending_) {
        return nullptr;
    }

    return &pending_;
}

const HeartMessage* MessageManager::current() const {
    if (!hasCurrent_) {
        return nullptr;
    }

    return &current_;
}

void MessageManager::clear() {
    pending_ = HeartMessage();
    current_ = HeartMessage();

    hasPending_ = false;
    hasCurrent_ = false;
}

bool MessageManager::isValid(const HeartMessage& message) {
    if (message.type == MessageType::Custom && message.text.length() == 0) {
        return false;
    }

    return true;
}