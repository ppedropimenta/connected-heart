#pragma once

#include <string>

#include "Display/DisplayRenderer.h"

class FakeDisplayRenderer : public DisplayRenderer {
public:
    enum class Screen {
        None,
        Startup,
        Connecting,
        Setup,
        WaitingForMessage,
        Notification,
        RevealHeart,
        Message
    };

    void showStartup() override {
        lastScreen = Screen::Startup;
    }

    void showConnecting() override {
        lastScreen = Screen::Connecting;
    }

    void showSetup() override {
        lastScreen = Screen::Setup;
    }

    void showWaitingForMessage(const ConnectionState connectionState) override {
        lastScreen = Screen::WaitingForMessage;
        lastConnectionState = connectionState;
    }

    void showNotification(const ConnectionState connectionState) override {
        lastScreen = Screen::Notification;
        lastConnectionState = connectionState;
    }

    void showRevealHeart(const ConnectionState connectionState) override {
        lastScreen = Screen::RevealHeart;
        lastConnectionState = connectionState;
    }

    void showMessage(const char* text, const ConnectionState connectionState) override {
        lastScreen = Screen::Message;
        lastMessageText = text;
        lastConnectionState = connectionState;
    }

    Screen lastScreen = Screen::None;
    ConnectionState lastConnectionState = ConnectionState::Connecting;
    std::string lastMessageText;
};
