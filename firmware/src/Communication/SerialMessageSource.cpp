#include "SerialMessageSource.h"

#include "Communication/MessageTextResolver.h"
#include "Core/Log.h"

void SerialMessageSource::begin() {
}

void SerialMessageSource::update() {
    while (Serial.available() > 0) {
        const char incoming = static_cast<char>(Serial.read());

        if (incoming == '\r') {
            continue;
        }

        if (incoming == '\n') {
            handleLine();
            lineLength_ = 0;
            continue;
        }

        if (lineLength_ + 1 < LineBufferCapacity) {
            lineBuffer_[lineLength_++] = incoming;
        }
    }
}

bool SerialMessageSource::hasMessage() const {
    return hasPending_;
}

HeartMessage SerialMessageSource::takeMessage() {
    hasPending_ = false;
    return pending_;
}

void SerialMessageSource::handleLine() {
    lineBuffer_[lineLength_] = '\0';

    if (lineLength_ == 0) {
        return;
    }

    pending_ = MessageTextResolver::resolve(lineBuffer_);
    hasPending_ = true;

    LOG_INFO("Serial message received");
}
