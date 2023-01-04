#pragma once
#ifndef LI3_BENCHMARK_H
#define LI3_BENCHMARK_H

#include "logger.h"

#define BENCHMARK_START(timer_name)                \
    g_autofree GTimer *timer_name = g_timer_new(); \
    g_timer_start(timer_name)

#define BENCHMARK_END(timer_name, log_string) \
    g_timer_stop(timer_name);                 \
    log_info(log_string, g_timer_elapsed(timer_name, NULL))

#endif //LI3_BENCHMARK_H
