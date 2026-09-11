#include "MessageManager.h"

#include "Core/Config.h"

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

void MessageManager::restoreCurrent(const HeartMessage& message) {
    current_.set(message);
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

    if (message.text()[0] == '\0') {
        return false;
    }

    return message.displayedTextLength() <= Config::MaximumMessageLength;
}
