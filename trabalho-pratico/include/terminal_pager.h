#pragma once
#ifndef LI3_TERMINAL_PAGER_H
#define LI3_TERMINAL_PAGER_H

#include <glib.h>

#define PAGER_LINES 10

void paginate(GPtrArray *lines);

#endif //LI3_TERMINAL_PAGER_H
