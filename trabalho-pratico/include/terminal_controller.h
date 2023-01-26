#pragma once
#ifndef LI3_TERMINAL_CONTROLLER_H
#define LI3_TERMINAL_CONTROLLER_H

#include <glib.h>

#define PAGER_LINES 10

void print_content(GPtrArray *lines);

void paginate(GPtrArray *lines);

int print_page_content(GPtrArray *lines, int page);

#endif //LI3_TERMINAL_CONTROLLER_H
