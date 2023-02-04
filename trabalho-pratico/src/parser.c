#include "parser.h"

#include "file_util.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 8192

void read_csv_file(FILE *stream, ApplyLineFunction *apply_line_function, void *apply_function_first_arg) {
    char line_buffer[BUFFER_SIZE];

    // We use fgets to read the lines to impose a limit on the line size
    
    if (fgets(line_buffer, BUFFER_SIZE, stream) == NULL) return; // skip CSV headers

    TokenIterator *iterator = init_semicolon_separated_token_iterator();

     while (fgets(line_buffer, BUFFER_SIZE, stream) != NULL) {
        format_input_line(line_buffer);

        token_iterator_set_current(iterator, line_buffer);
        apply_line_function(apply_function_first_arg, iterator);
    }

    token_iterator_free(iterator);
}
