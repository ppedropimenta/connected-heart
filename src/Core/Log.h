/**
* @file Log.h
 * @brief Compile-time configurable logging macros.
 *
 * This module provides lightweight logging macros for debugging and
 * diagnostics over the serial interface.
 *
 * Log levels:
 * - 0: Logging disabled.
 * - 1: Error messages only.
 * - 2: Error and warning messages.
 * - 3: Error, warning, and informational messages (default).
 * - 4: All messages, including debug output.
 *
 * The log level can be overridden using a compiler definition, e.g.:
 * @code
 * build_flags =
 *     -DLOG_LEVEL=4
 * @endcode
 */

#pragma once

/**
 * On the ESP32 build this logs over Serial with a millis() timestamp. On
 * the host build (no ARDUINO define) it falls back to stdio so the message
 * layer and Application stay testable without Arduino.
 */
#ifdef ARDUINO
#include <Arduino.h>
#define LOG_PRINTF(fmt, ...) Serial.printf(fmt, millis(), ##__VA_ARGS__)
#else
#include <cstdio>
#define LOG_PRINTF(fmt, ...) std::printf(fmt, 0UL, ##__VA_ARGS__)
#endif

#ifndef LOG_LEVEL
#define LOG_LEVEL 3
#endif

#define LOG_ERROR_LEVEL 1
#define LOG_WARN_LEVEL  2
#define LOG_INFO_LEVEL  3
#define LOG_DEBUG_LEVEL 4

#if LOG_LEVEL >= LOG_ERROR_LEVEL
#define LOG_ERROR(fmt, ...) LOG_PRINTF("[%lu] ERROR: " fmt "\n", ##__VA_ARGS__)
#else
#define LOG_ERROR(...)
#endif

#if LOG_LEVEL >= LOG_WARN_LEVEL
#define LOG_WARN(fmt, ...) LOG_PRINTF("[%lu] WARN: " fmt "\n", ##__VA_ARGS__)
#else
#define LOG_WARN(...)
#endif

#if LOG_LEVEL >= LOG_INFO_LEVEL
#define LOG_INFO(fmt, ...) LOG_PRINTF("[%lu] INFO: " fmt "\n", ##__VA_ARGS__)
#else
#define LOG_INFO(...)
#endif

#if LOG_LEVEL >= LOG_DEBUG_LEVEL
#define LOG_DEBUG(fmt, ...) LOG_PRINTF("[%lu] DEBUG: " fmt "\n", ##__VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif