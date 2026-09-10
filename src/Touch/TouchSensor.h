/**
* @file TouchSensor.h
 * @brief Interface for the Connected Heart touch surface.
 */

#pragma once

#include "Core/Module.h"

class TouchSensor : public Module {
public:
    /**
     * @brief Returns whether a debounced press occurred since the last call.
     *
     * A press is reported at most once.
     */
    virtual bool wasPressed() = 0;
};
