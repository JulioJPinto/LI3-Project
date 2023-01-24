#pragma once
#ifndef LI3_BENCHMARK_H
#define LI3_BENCHMARK_H

#include "logger.h"
#include "terminal_colors.h"

#ifndef OMIT_BENCHMARK_LOGGING
#define BENCHMARK_LOG(string, ...) \
    log_info(TERMINAL_DARK_GRAY string TERMINAL_RESET, __VA_ARGS__)
#else
#define BENCHMARK_LOG(...)
#endif

#define BENCHMARK_START(timer_name)                \
    g_autofree GTimer *timer_name = g_timer_new(); \
    g_timer_start(timer_name)

#define BENCHMARK_END(timer_name, log_string) \
    g_timer_stop(timer_name);                 \
    BENCHMARK_LOG(log_string, g_timer_elapsed(timer_name, NULL))

#endif //LI3_BENCHMARK_H
