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

struct Program {
    Catalog *catalog;
    ProgramState state;
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

void program_ask_for_commands(Program *program) {
    program->state = PROGRAM_STATE_WAITING_FOR_COMMANDS;
    char *input = readline("> ");

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

    add_history(input);

    free(input);
}

void run_program(Program *program, char **args, int arg_size) {
    if (arg_size >= 2) {
        char *dataset_folder_path = args[1];
        char *queries_file_path = args[2];

        program_load_dataset(program, dataset_folder_path);
        program_run_queries_from_file(program, queries_file_path);
    } else {
        rl_bind_key('\t', rl_complete);
        using_history();

        program_ask_for_dataset_path(program);
        while (program->state != PROGRAM_STATE_EXITING) {
            program_ask_for_commands(program);
        }
    }
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
    // TODO: Make output to console or file
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
