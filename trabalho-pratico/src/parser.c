#include "parser.h"

#include <stdio.h>

void read_file(FILE *stream, ParserLineFunction *parse_line_function, ApplyFunction *apply_function, void *apply_function_first_arg) {
    char line_buffer[1024];

    if (fgets(line_buffer, 1024, stream) == NULL) {
        return;
    }

    while (fgets(line_buffer, 1024, stream) != NULL) {
        void *parsed_line = parse_line_function(line_buffer, ';');
        apply_function(apply_function_first_arg, parsed_line);
    }
}
