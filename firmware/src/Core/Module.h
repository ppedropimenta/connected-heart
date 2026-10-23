/**
* @file Module.h
 * @brief Defines the common interface for system modules.
 *
 * A module represents an independent subsystem of the application.
 * Each module is initialized once through begin() and updated
 * periodically through update().
 */

#pragma once

class Module {
public:
    virtual ~Module() = default;

    /**
     * @brief Initializes the module.
     *
     * Called once during system startup, before update() is called.
     */
    virtual void begin() = 0;

    /**
     * @brief Performs the module's periodic processing.
     *
     * Called repeatedly from the main application loop. Implementations
     * should avoid long blocking operations.
     */
    virtual void update() = 0;
};
