#pragma once
#ifndef LI3_FILE_UTIL_H
#define LI3_FILE_UTIL_H

#include <stdio.h>

/**
 * Opens a file in a given folder path and the file name.
 * If the file does not exist, the program exits with an error message.
 */
FILE *open_file_folder(const char *folder_path, const char *file_name);

/**
 * Opens a file with the given file name.
 * If the file does not exist, the program exits with an error message.
 */
FILE *open_file(const char *file_name);

/**
 * Removes the trailing newline character from fgets input
 */
void format_fgets_input_line(char *line_buffer);

/**
 * Creates the output folder if it does not exist.
 */
void create_output_folder_if_not_exists(void);

/**
 * Creates a file for the output of a command.
 * The file is created in the output folder.
 * The file name is command<command_number>_output.txt
 * The file should be closed after use.
 */
FILE *create_command_output_file(int command_number);

#endif //LI3_FILE_UTIL_H
