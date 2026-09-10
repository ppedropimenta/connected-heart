/**
* @file LedPatternSelector.h
 * @brief Chooses which LedPattern the notification LED should show.
 */

#pragma once

#include "Led/LedPattern.h"
#include "State/ConnectionState.h"
#include "State/DeviceState.h"

/**
 * @brief Maps the current DeviceState and ConnectionState to a LedPattern.
 *
 * The device state selects the pattern. ConnectionError only replaces a
 * pattern that would otherwise be Off, so a pending Notification or a
 * Reveal keeps its own rhythm even while the connection is down; the
 * display already carries the connection indicator elsewhere on those
 * screens.
 */
class LedPatternSelector {
public:
    static LedPattern select(DeviceState deviceState, ConnectionState connectionState);
};
