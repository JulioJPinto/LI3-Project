#include "program.h"

#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "benchmark.h"
#include "catalog.h"
#include "file_util.h"
#include "string_util.h"
#include "logger.h"
#include "query_manager.h"
#include "parser.h"

typedef enum ProgramState {
    PROGRAM_STATE_RUNNING,
    PROGRAM_STATE_WAITING_FOR_DATASET_INPUT,
    PROGRAM_STATE_WAITING_FOR_COMMANDS,
    PROGRAM_STATE_VIEWING_QUERY_RESULT,
    PROGRAM_STATE_EXITING,
} ProgramState;

typedef enum ProgramMode {
    WAITING_FOR_MODE,
    RUNNING_IN_BATCH_MODE,
    RUNNING_IN_ITERATIVE_MODE_TO_FILE,
    RUNNING_IN_ITERATIVE_MODE,
} ProgramMode;

struct Program {
    ProgramFlags *flags;
    Catalog *catalog;
    ProgramState state;
    ProgramMode mode;
    int current_query_id;
};

typedef struct ProgramCommand {
    char *name;
    char *description;
    void (*function)(Program *program, char **args, int arg_size);
} ProgramCommand;

void program_run_queries_from_file_command(Program *program, char **args, int arg_size) {
    if (arg_size < 2) {
        log_warning("Use 'file <file_path>'\n");
        return;
    }

    char *input_file_path = args[1];

    if (!program_run_queries_from_file(program, input_file_path)) {
        log_warning("Failed to run queries from file '%s'\n", input_file_path);
    }
}

void program_run_help_command(Program *program, char **args, int arg_size);

void program_exit_command(Program *program, char **args, int arg_size) {
    (void) args;
    (void) arg_size;

    program->state = PROGRAM_STATE_EXITING;
}

const ProgramCommand program_commands[] = {
        {"file", "Runs all the queries from a file", program_run_queries_from_file_command},
        {"help", "Shows this help message", program_run_help_command},
        {"exit", "Exits the program", program_exit_command},
};

const int program_commands_size = sizeof(program_commands) / sizeof(ProgramCommand);

void program_run_help_command(Program *program, char **args, int arg_size) {
    (void) program;
    (void) args;
    (void) arg_size;

    log_info("Available commands:\n");
    log_info("  <query_id> <query> - Runs a query\n");

    for (int i = 0; i < program_commands_size; i++) {
        log_info("  %s - %s\n", program_commands[i].name, program_commands[i].description);
    }
}

Program *create_program(ProgramFlags *flags) {
    Program *program = malloc(sizeof(Program));
    program->catalog = create_catalog();
    program->state = PROGRAM_STATE_RUNNING;
    program->mode = WAITING_FOR_MODE;
    program->current_query_id = 0;
    program->flags = flags;
    return program;
}

void free_program(Program *program) {
    free_catalog(program->catalog);
    free(program);
}

void program_ask_for_dataset_path(Program *program) {
    program->state = PROGRAM_STATE_WAITING_FOR_DATASET_INPUT;

    gboolean loaded = FALSE;
    while (!loaded) {
        char *input = readline("Please enter the path to the dataset folder (default: datasets/data-regular): ");

        if (IS_EMPTY(input)) {
            strcpy(input, "datasets/data-regular");
        }

        loaded = program_load_dataset(program, input);
        free(input);
    }
}

void execute_command(Program *program, char *input) {
    char **args = g_strsplit(input, " ", 0);
    int arg_size = (int) g_strv_length(args);

    if (arg_size == 0) return;

    for (int i = 0; i < program_commands_size; i++) {
        if (strcmp(program_commands[i].name, args[0]) == 0) {
            program_commands[i].function(program, args, arg_size);
            return;
        }
    }

    if (isdigit(args[0][0])) {
        program_run_query(program, input);
    } else {
        log_warning("Invalid command '%s'\n", args[0]);
    }
}

void program_ask_for_commands(Program *program) {
    program->state = PROGRAM_STATE_WAITING_FOR_COMMANDS;
    char *input = readline("> ");

    execute_command(program, input);

    add_history(input);
    free(input);
}

int start_program(Program *program, GPtrArray *program_args) {
    if (program_args->len >= 2) {
        program->mode = RUNNING_IN_BATCH_MODE;
        char *dataset_folder_path = g_ptr_array_index(program_args, 0);
        char *queries_file_path = g_ptr_array_index(program_args, 1);

        if (!program_load_dataset(program, dataset_folder_path))
            return EXIT_FAILURE;
        if (!program_run_queries_from_file(program, queries_file_path))
            return EXIT_FAILURE;

    } else {
        program->mode = RUNNING_IN_ITERATIVE_MODE;
        rl_bind_key('\t', rl_complete);
        using_history();

        program_ask_for_dataset_path(program);
        while (program->state != PROGRAM_STATE_EXITING) {
            program_ask_for_commands(program);
        }
    }

    return EXIT_SUCCESS;
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

    char *lazy_loading_value_string = get_program_flag_value(program->flags, "lazy-loading", "true");
    if (strcmp(lazy_loading_value_string, "true") != 0)
        catalog_force_eager_indexing(catalog);

    fclose(users_file);
    fclose(drivers_file);
    fclose(rides_file);

    return TRUE;
}

void run_query_for_terminal(Catalog *catalog, char* query, int query_number) {
    fprintf(stdout, "=== Query number: #%d =====================\n", query_number);
    parse_and_run_query(catalog, stdout, query);
    fprintf(stdout, "===========================================\n");
}

void run_query_for_output_folder(Catalog *catalog, char* query, int query_number) {
    create_output_folder_if_not_exists();
    FILE *output_file = create_command_output_file(query_number);
    parse_and_run_query(catalog, output_file, query);
    fclose(output_file);
}

void program_run_query(Program *program, char *query) {
    Catalog *catalog = program->catalog;
    int query_number = ++program->current_query_id;

    if (*query == '#') {
        return;
    }

    if (program->mode == RUNNING_IN_ITERATIVE_MODE) {
        run_query_for_terminal(catalog, query, query_number);
    } else {
        run_query_for_output_folder(catalog, query, query_number);        
    }
}

gboolean program_run_queries_from_file(Program *program, char *input_file_path) {
    FILE *input_file = open_file(input_file_path);
    if (input_file == NULL) {
        return FALSE;
    }

    BENCHMARK_START(input_file_execution_timer);

    char line_buffer[65536];

    program->mode = RUNNING_IN_ITERATIVE_MODE_TO_FILE;

    int aux_id = program->current_query_id;
    program->current_query_id = 0;

    while (fgets(line_buffer, 65536, input_file)) {
        format_fgets_input_line(line_buffer);
        program_run_query(program, line_buffer);
    }

    program->mode = RUNNING_IN_ITERATIVE_MODE;

    g_timer_stop(input_file_execution_timer);
    log_info("%d queries from %s executed in %f seconds\n", program->current_query_id, input_file_path, g_timer_elapsed(input_file_execution_timer, NULL));
    program->current_query_id = aux_id;

    fclose(input_file);

    return TRUE;
}
