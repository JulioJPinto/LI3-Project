#pragma once
#ifndef LI3_LOGGER_H
#define LI3_LOGGER_H

#include "terminal_colors.h"

/**
 * Logs a debug message if the DEBUG flag is defined.
 */
void log_debug(const char *format, ...);

/**
 * Logs an info message.
 */
void log_info(const char *format, ...);

/**
 * Logs a warning message.
 */
void log_warning(const char *format, ...);

#define LOG_WARNING_VA(text, ...) log_warning(TERMINAL_RED "Warning: " text TERMINAL_RESET "\n", __VA_ARGS__)
#define LOG_WARNING(text) LOG_WARNING_VA(text, "")

/**
 * Logs an error message and exits the program.
 */
void log_panic(const char *format, ...);

#endif //LI3_LOGGER_H
