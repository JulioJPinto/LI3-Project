#include "program.h"

#include "benchmark.h"
#include "catalog.h"
#include "file_util.h"
#include "logger.h"
#include "query_manager.h"
#include "parser.h"

typedef enum ProgramState {
    PROGRAM_STATE_RUNNING,
    PROGRAM_STATE_WAITING_FOR_DATASET_INPUT,
    PROGRAM_STATE_WAITING_FOR_QUERY_INPUT,
    PROGRAM_STATE_VIEWING_QUERY_RESULT,
} ProgramState;

struct Program {
    Catalog *catalog;
    ProgramState state;
    int current_query_id;
};

Program *create_program(void) {
    Program *program = malloc(sizeof(Program));
    program->catalog = create_catalog();
    program->state = PROGRAM_STATE_RUNNING;
    program->current_query_id = 0;
    return program;
}

void free_program(Program *program) {
    free_catalog(program->catalog);
    free(program);
}

gboolean program_load_dataset(Program *program, char *dataset_folder_path) {
    FILE *users_file = open_file_folder(dataset_folder_path, "users.csv");
    FILE *drivers_file = open_file_folder(dataset_folder_path, "drivers.csv");
    FILE *rides_file = open_file_folder(dataset_folder_path, "rides.csv");

    if (users_file == NULL || drivers_file == NULL || rides_file == NULL) {
        return FALSE;
    }

    Catalog *catalog = program->catalog;

    BENCHMARK_START(load_timer);
    read_file(users_file, parse_and_register_user, catalog);
    BENCHMARK_END(load_timer, "Load users time:        %f seconds\n");

    g_timer_start(load_timer);
    read_file(drivers_file, parse_and_register_driver, catalog);
    BENCHMARK_END(load_timer, "Load drivers time:      %f seconds\n");

    g_timer_start(load_timer);
    read_file(rides_file, parse_and_register_ride, catalog);
    BENCHMARK_END(load_timer, "Load rides time:        %f seconds\n");

    g_timer_start(load_timer);
    notify_stop_registering(catalog);
    BENCHMARK_END(load_timer, "Final indexing time:    %f seconds\n");

    fclose(users_file);
    fclose(drivers_file);
    fclose(rides_file);

    return TRUE;
}

void program_run_query(Program *program, char *query) {
    Catalog *catalog = program->catalog;
    int query_number = ++program->current_query_id;

    if (*query == '#') {
        return;
    }

    create_output_folder_if_not_exists();
    FILE *output_file = create_command_output_file(query_number);

    parse_and_run_query(catalog, output_file, query);

    fclose(output_file);
}

gboolean program_run_queries_from_file(Program *program, char *input_file_path) {
    FILE *input_file = open_file(input_file_path);
    if (input_file == NULL) {
        return FALSE;
    }

    BENCHMARK_START(input_file_execution_timer);

    char line_buffer[65536];

    while (fgets(line_buffer, 65536, input_file)) {
        format_fgets_input_line(line_buffer);
        program_run_query(program, line_buffer);
    }
    
    g_timer_stop(input_file_execution_timer);
    log_info("%d queries from %s executed in %f seconds\n", program->current_query_id, input_file_path, g_timer_elapsed(input_file_execution_timer, NULL));

    fclose(input_file);
    
    return TRUE;
}
