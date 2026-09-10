#include "MessageManager.h"

#include <new>

#include "TextMetrics.h"
#include "Core/Config.h"

MessageManager::Slot::~Slot() {
    reset();
}

void MessageManager::Slot::set(const HeartMessage& message) {
    reset();
    new (storage_) HeartMessage(message);
    present_ = true;
}

void MessageManager::Slot::reset() {
    if (present_) {
        reinterpret_cast<HeartMessage*>(storage_)->~HeartMessage();
        present_ = false;
    }
}

const HeartMessage* MessageManager::Slot::get() const {
    if (!present_) {
        return nullptr;
    }

    return reinterpret_cast<const HeartMessage*>(storage_);
}

bool MessageManager::receive(const HeartMessage& message) {
    if (!isValid(message)) {
        return false;
    }

    pending_.set(message);

    return true;
}

bool MessageManager::revealPending() {
    if (!pending_.present()) {
        return false;
    }

    current_.set(*pending_.get());
    pending_.reset();

    return true;
}

bool MessageManager::hasPending() const {
    return pending_.present();
}

bool MessageManager::hasCurrent() const {
    return current_.present();
}

const HeartMessage* MessageManager::pending() const {
    return pending_.get();
}

const HeartMessage* MessageManager::current() const {
    return current_.get();
}

void MessageManager::clear() {
    pending_.reset();
    current_.reset();
}

bool MessageManager::isValid(const HeartMessage& message) {
    if (message.kind() != MessageKind::Custom) {
        return true;
    }

    const char* text = message.text();

    if (text[0] == '\0') {
        return false;
    }

    return TextMetrics::displayedLength(text) <= Config::MaximumMessageLength;
}
