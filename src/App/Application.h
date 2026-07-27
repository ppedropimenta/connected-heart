/**
* @file Application.h
 * @brief Defines the main application controller.
 *
 * The Application class owns the system modules and coordinates
 * their initialization and periodic execution.
 */

#pragma once

#include <array>

#include "Core/Module.h"
#include "Display/EpaperDisplay.h"
#include "Led/Led.h"

class Application {
public:
    void begin();
    void update();

private:
    Led led_;
    EpaperDisplay display_;

    std::array<Module*, 2> modules_{
        &led_,
        &display_
    };
};
