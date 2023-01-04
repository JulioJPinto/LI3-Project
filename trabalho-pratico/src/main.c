#include "benchmark.h"
#include "program.h"

/**
 * Main entry point
 */
int main(int argc, char **argv) {
    log_debug("Running on debug mode\n");

    if (argc < 2) {
        // TODO: interactive mode
        log_warning("Usage: %s <dataset_folder_path> <queries_file_path>\n", argv[0]);
        return 0;
    }

    BENCHMARK_START(global_timer);
    Program *program = create_program();

    char *dataset_folder_path = argv[1];
    char *queries_file_path = argv[2];

    program_load_dataset(program, dataset_folder_path);
    program_run_queries_from_file(program, queries_file_path);

    free_program(program);

    log_info("Total runtime:          %lf seconds\n", g_timer_elapsed(global_timer, NULL));

    return 0;
}
