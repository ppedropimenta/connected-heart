#pragma once

#include "Core/Module.h"

class TouchSensor : public Module
{
public:
    void begin() override;
    void update() override;

    bool wasPressed();

private:
    bool stableState_ = false;
    bool rawState_ = false;
    bool pressEvent_ = false;

    unsigned long lastChangeAt_ = 0;
};
