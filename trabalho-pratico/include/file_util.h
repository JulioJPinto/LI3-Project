#pragma once
#ifndef LI3_FILE_UTIL_H
#define LI3_FILE_UTIL_H

#include <stdio.h>

/*
 * Opens a file in a given folder path and the file name.
 * If the file does not exist, the program exits with an error message.
 */
FILE *open_file_folder(const char *folder_path, const char *file_name);

/*
 * Opens a file with the given file name.
 * If the file does not exist, the program exits with an error message.
 */
FILE *open_file(const char *file_name);

/*
 * Removes the trailing newline character from fgets input
 */
void format_fgets_input_line(char *line_buffer);

#endif //LI3_FILE_UTIL_H
