#include "parser.h"

#include "file_util.h"
#include <stdio.h>

void read_file(FILE *stream, ApplyLineFunction *apply_line_function, void *apply_function_first_arg) {
    char line_buffer[1024];

    if (fgets(line_buffer, 1024, stream) == NULL) {
        return;
    }

    while (fgets(line_buffer, 1024, stream) != NULL) {
        format_fgets_input_line(line_buffer);
        apply_line_function(apply_function_first_arg, line_buffer, ';');
    }
}
