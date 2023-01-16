#include "benchmark.h"
#include "program.h"
#include "program_flags.h"

/**
 * Main entry point
 */
int main(int argc, char **argv) {
    log_debug("Running on debug mode\n");

    BENCHMARK_START(global_timer);

    GPtrArray *program_args = g_ptr_array_new();
    for (int i = 1; i < argc; i++) {
        g_ptr_array_add(program_args, argv[i]);
    }

    ProgramFlags *program_flags = retrieve_program_flags(program_args);

    Program *program = create_program(program_flags);

    int result = start_program(program, program_args);

    g_ptr_array_free(program_args, TRUE);
    free_program_flags(program_flags);
    free_program(program);

    log_info("Total runtime:          %lf seconds\n", g_timer_elapsed(global_timer, NULL));

    return result;
}
