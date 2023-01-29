#pragma once
#ifndef LI3_TERMINAL_CONTROLLER_H
#define LI3_TERMINAL_CONTROLLER_H

#include <glib.h>

#define PAGER_LINES 10

/**
 * Prints the given lines to the terminal.
 * If the `lines` length is higher than `PAGER_LINES`, the lines are printed in the terminal in a paginated way.
 */
void print_content(GPtrArray *lines);

/**
 * Prints the given lines to the terminal in a paginated way.
 * The user can navigate through the pages using the terminal.
 */
void paginate(GPtrArray *lines);

/**
 * Prints the lines from the given page to the terminal.
 * The lines from the page are calculated using the `PAGER_LINES` constant.
 * Returns how many lines were printed.
 */
int print_page_content(GPtrArray *lines, int page);

#endif //LI3_TERMINAL_CONTROLLER_H
