#include "HeartMessageSlot.h"

#include <new>

HeartMessageSlot::~HeartMessageSlot() {
    reset();
}

void HeartMessageSlot::set(const HeartMessage& message) {
    reset();
    new (storage_) HeartMessage(message);
    present_ = true;
}

void HeartMessageSlot::reset() {
    if (present_) {
        reinterpret_cast<HeartMessage*>(storage_)->~HeartMessage();
        present_ = false;
    }
}

const HeartMessage* HeartMessageSlot::get() const {
    if (!present_) {
        return nullptr;
    }

    return reinterpret_cast<const HeartMessage*>(storage_);
}
