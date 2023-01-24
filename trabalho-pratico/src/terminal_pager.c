#include <math.h>
#include <printf.h>
#include <readline/readline.h>
#include "terminal_pager.h"
#include "struct_util.h"

void clear_terminal_lines(int number_of_lines) {
    for (int i = 0; i < number_of_lines; i++) {
        printf("\x1b[1F");
        printf("\x1b[2K");
        fflush(stdout);
    }
}

int print_page_content(GPtrArray *lines, int page) {
    int start = (page - 1) * PAGER_LINES;
    int end = MIN(start + PAGER_LINES, (int) lines->len);

    for (int i = start; i < end; i++) {
        printf("%s", (char *) g_ptr_array_index(lines, i));
    }

    return end - start;
}

void paginate(GPtrArray *lines) {
    if (lines == NULL || lines->len <= 0) return;

    int currentPage = 1;
    int totalPages = ceil((double) lines->len / PAGER_LINES);

    int continue_paging = TRUE;
    while (continue_paging) {
        int number_of_lines_printed = print_page_content(lines, currentPage);
        printf("\nPage %d of %d\n", currentPage, totalPages);
        char *input = readline("Go to page ('n', 'p', 'q', 'number'): ");

        int parse_error = 0;
        int new_page = parse_int_safe(input, &parse_error);

        if (*input == 'q') {
            continue_paging = FALSE;
        } else if (*input == 'n') {
            if (currentPage < totalPages) currentPage++;
        } else if (*input == 'p') {
            if (currentPage > 1) currentPage--;
        } else if (!parse_error) {
            if (new_page >= 1 && new_page <= totalPages) currentPage = new_page;
        }

        free(input);

        clear_terminal_lines(3); // Page number line + Go to page line + Enter from input
        if (continue_paging) {
            clear_terminal_lines(number_of_lines_printed); // Only delete output lines if user continues paging
        }
    }
}
