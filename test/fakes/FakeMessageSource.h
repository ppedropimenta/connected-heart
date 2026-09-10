#pragma once

#include <deque>

#include "Communication/MessageSource.h"

class FakeMessageSource : public MessageSource {
public:
    void begin() override {}
    void update() override {}

    bool hasMessage() const override {
        return !queue_.empty();
    }

    HeartMessage takeMessage() override {
        const HeartMessage message = queue_.front();
        queue_.pop_front();
        return message;
    }

    /**
     * @brief Queues a message as if it had just arrived from the network.
     */
    void enqueue(const HeartMessage& message) {
        queue_.push_back(message);
    }

private:
    std::deque<HeartMessage> queue_;
};
