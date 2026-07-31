#pragma once

#include "Core/Module.h"

class WifiManager : public Module {
public:
    void begin() override;
    void update() override;

    bool isConnected() const;
    bool justConnected();
    bool justDisconnected();
};
