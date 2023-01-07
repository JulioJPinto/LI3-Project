#include "benchmark.h"
#include "program.h"

/**
 * Main entry point
 */
int main(int argc, char **argv) {
    log_debug("Running on debug mode\n");

    BENCHMARK_START(global_timer);
    Program *program = create_program();

    int result = run_program(program, argv, argc);

    free_program(program);

    log_info("Total runtime:          %lf seconds\n", g_timer_elapsed(global_timer, NULL));
    
    return result;


    return 0;
}
