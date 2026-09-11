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

    void reportOutcome(const bool accepted, const bool wasPendingDisplaced) override {
        outcomeReportCount++;
        lastAccepted = accepted;
        lastWasPendingDisplaced = wasPendingDisplaced;
    }

    int outcomeReportCount = 0;
    bool lastAccepted = false;
    bool lastWasPendingDisplaced = false;

private:
    std::deque<HeartMessage> queue_;
};
