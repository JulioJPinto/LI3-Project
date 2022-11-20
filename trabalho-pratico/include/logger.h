#pragma once
#ifndef LI3_LOGGER_H
#define LI3_LOGGER_H

/*
 * Logs a debug message if the DEBUG flag is defined.
 */
void log_debug(const char *format, ...);

/*
 * Logs an info message.
 */
void log_info(const char *format, ...);

/*
 * Logs a warning message.
 */
void log_warning(const char *format, ...);

/*
 * Logs an error message and exits the program.
 */
void log_panic(const char *format, ...);

#endif //LI3_LOGGER_H
