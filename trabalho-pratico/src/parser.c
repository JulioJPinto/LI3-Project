#include "parser.h"

#include <stdio.h>

void read_file(FILE *stream, ApplyLineFunction *apply_line_function, void *apply_function_first_arg) {
    char line_buffer[1024];

    if (fgets(line_buffer, 1024, stream) == NULL) {
        return;
    }

    while (fgets(line_buffer, 1024, stream) != NULL) {
        apply_line_function(apply_function_first_arg, line_buffer, ';');
    }
}
