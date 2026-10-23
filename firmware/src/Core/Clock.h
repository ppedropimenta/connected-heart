/**
* @file Clock.h
 * @brief Interface for reading elapsed time.
 *
 * Application reads time through this seam instead of the Arduino clock
 * directly, so reveal and timeout behaviour can be tested without waiting
 * in real time.
 */

#pragma once

class Clock {
public:
    virtual ~Clock() = default;

    /**
     * @brief Milliseconds elapsed since some fixed but arbitrary reference
     * point.
     *
     * Only differences between two calls are meaningful.
     */
    virtual unsigned long millis() const = 0;
};
