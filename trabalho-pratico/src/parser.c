#define _GNU_SOURCE

#include "parser.h"

#include "file_util.h"
#include <stdio.h>
#include <stdlib.h>

void read_csv_file(FILE *stream, ApplyLineFunction *apply_line_function, void *apply_function_first_arg) {
    char *line_buffer = malloc(1024);
    size_t line_buffer_size = 1024;

    // By using getline, we are assuming the input has not big enough lines to run out of memory.

    if (getline(&line_buffer, &line_buffer_size, stream) < 0) { // skip CSV headers
        return;
    }

    TokenIterator *iterator = init_semicolon_separated_token_iterator();

    while (getline(&line_buffer, &line_buffer_size, stream) >= 0) {
        format_input_line(line_buffer);

        token_iterator_set_current(iterator, line_buffer);
        apply_line_function(apply_function_first_arg, iterator);
    }

    token_iterator_free(iterator);
    free(line_buffer);
}
