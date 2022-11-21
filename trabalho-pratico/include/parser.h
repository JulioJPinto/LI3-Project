#pragma once
#ifndef LI3_PARSER_H
#define LI3_PARSER_H

#include <stdio.h>

typedef void *(ParserLineFunction) (char *, char *);

typedef void(ApplyFunction)(void *, void *);

/**
 * Function that reads the CSV file and parses each line according to the given parse_line_function.
 * The parsed line is then passed to the apply_function with the given first argument and the parsed line as the second argument.
 */
void read_file(FILE *stream, ParserLineFunction *parse_line_function, ApplyFunction *apply_function, void *apply_function_first_arg);

#endif //LI3_PARSER_H
