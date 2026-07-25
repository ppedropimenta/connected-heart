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
#include "Led/Led.h"

class Application final {
public:
    /**
     * @brief Initializes the application and all registered modules.
     *
     * This function must be called once from Arduino's setup().
     */
    void begin();

    /**
     * @brief Executes one application cycle.
     *
     * This function must be called repeatedly from Arduino's loop().
     */
    void update();

private:
    Led led_;

    std::array<Module*, 1> modules_{
        &led_
    };
};
