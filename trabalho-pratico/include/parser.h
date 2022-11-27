#pragma once
#ifndef LI3_PARSER_H
#define LI3_PARSER_H

#include <stdio.h>

/**
 * Function definition that parses a line from the file to user/driver/ride structure.
 * char *line: line to parse
 * char *delimiter: delimiter to use in parsing (';')
 */
typedef void *(ParserLineFunction) (char *, char);

/**
 * Function definition to apply after the line is parsed.
 * void *arg0: the first argument to pass to the function (the catalog as ApplyFunction is typically a register(Catalog*, User/Driver/Ride))
 * void *arg1: the struct already parsed from the line (User, Driver or Ride)
 */
typedef void(ApplyFunction)(void *, void *);

/**
 * Function that reads the CSV file and parses each line according to the given parse_line_function.
 * The parsed line is then passed to the apply_function with the given first argument and the parsed line as the second argument.
 */
void read_file(FILE *stream, ParserLineFunction *parse_line_function, ApplyFunction *apply_function, void *apply_function_first_arg);

#endif //LI3_PARSER_H
