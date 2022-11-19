#pragma once
#ifndef LI3_PARSER_H
#define LI3_PARSER_H

#include <stdio.h>

#include "struct_util.h"

typedef void *(ParserLineFunction) (char *);

typedef void(ApplyFunction)(void *, void *);

/*
 * Function that reads the CSV file and parses each line to a pre-defined struct
 */
void read_file(FILE *stream, ParserLineFunction *parse_line_function, ApplyFunction *apply_function, void *apply_function_first_arg);

#endif //LI3_PARSER_H
