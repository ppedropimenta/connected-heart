/**
* @file Led.h
 * @brief Interface for the Connected Heart notification LED.
 *
 * Led knows nothing about device states or application flow; it only
 * renders whichever LedPattern it is asked to show.
 */

#pragma once

#include "Core/Module.h"
#include "Led/LedPattern.h"

class Led : public Module {
public:
    /**
     * @brief Shows the given pattern, replacing whatever was showing.
     */
    virtual void show(LedPattern pattern) = 0;
};
