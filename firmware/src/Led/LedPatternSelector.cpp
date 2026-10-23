#include "LedPatternSelector.h"

namespace {
    LedPattern patternForDeviceState(const DeviceState deviceState) {
        switch (deviceState) {
            case DeviceState::Setup:
                return LedPattern::Setup;

            case DeviceState::Waking:
                return LedPattern::Connecting;

            case DeviceState::Notification:
                return LedPattern::NotificationPulse;

            case DeviceState::RevealingMessage:
                return LedPattern::RevealHeartbeat;

            case DeviceState::Boot:
            case DeviceState::WaitingForMessage:
            case DeviceState::DisplayingMessage:
                return LedPattern::Off;
        }

        return LedPattern::Off;
    }
}

LedPattern LedPatternSelector::select(
    const DeviceState deviceState,
    const ConnectionState connectionState
) {
    const LedPattern pattern = patternForDeviceState(deviceState);

    if (pattern == LedPattern::Off && connectionState == ConnectionState::Error) {
        return LedPattern::ConnectionError;
    }

    return pattern;
}
