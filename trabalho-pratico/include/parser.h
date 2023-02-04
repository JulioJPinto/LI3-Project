#pragma once
#ifndef LI3_PARSER_H
#define LI3_PARSER_H

#include <stdio.h>
#include "token_iterator.h"

/**
 * Function definition that every line is applied to.
 * This is used to abstract catalog registration of users, drivers and rides.
 * void *arg: the suposed catalog
 * TokenIterator *iterator: the iterator that splits the line into tokens
 */
typedef void(ApplyLineFunction)(void *, TokenIterator *);

/**
 * Function that reads the CSV file and applies apply_line_function for each line.
 * The apply_function_first_arg is passed as the first argument to apply_line_function.
 */
void read_csv_file(FILE *stream, ApplyLineFunction *apply_line_function, void *apply_function_first_arg);

#endif //LI3_PARSER_H
