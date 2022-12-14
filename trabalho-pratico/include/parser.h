#pragma once
#ifndef LI3_PARSER_H
#define LI3_PARSER_H

#include <stdio.h>

/**
 * Function definition that every line is applied to.
 * This is used to abstract catalog registration of users, drivers and rides.
 * void *arg: catalog
 * char *line: line to parse
 * char *delimiter: delimiter to use in parsing (';')
 */
typedef void(ApplyLineFunction)(void *, char *, char);

/**
 * Function that reads the CSV file and applies apply_line_function for each line.
 * The apply_function_first_arg is passed as the first argument to apply_line_function.
 */
void read_file(FILE *stream, ApplyLineFunction *apply_line_function, void *apply_function_first_arg);

#endif //LI3_PARSER_H
