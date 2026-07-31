#pragma once

#include "EpaperDisplay.h"
#include "../Message/HeartMessage.h"

class DisplayRenderer {
public:
    explicit DisplayRenderer(EpaperDisplay& display);

    void showStartup();
    void showConnecting();
    void showWaitingForMessage();
    void showNotification();
    void showRevealHeart();
    void showMessage(const HeartMessage& message);
    void showConnectionError();

private:
    EpaperDisplay& display_;
};