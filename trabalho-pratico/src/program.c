#include "program.h"

#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "benchmark.h"
#include "catalog.h"
#include "catalog_loader.h"
#include "file_util.h"
#include "logger.h"
#include "query_manager.h"
#include "string_util.h"
#include "terminal_controller.h"
#include "program_commands.h"

/**
 * Struct that holds information about the program.
 */
struct Program {
    ProgramFlags *flags;
    Catalog *catalog;
    ProgramState state;

    gboolean should_exit;
};

void program_set_state(Program *program, ProgramState state) {
    program->state = state;
}

gboolean program_should_exit(Program *program) {
    return program->should_exit;
}

void program_set_should_exit(Program *program, gboolean should_exit) {
    program->should_exit = should_exit;
}

Program *create_program(ProgramFlags *flags) {
    Program *program = malloc(sizeof(Program));
    program->catalog = create_catalog();
    program->state = PROGRAM_STATE_RUNNING;
    program->flags = flags;
    program->should_exit = TRUE;
    return program;
}

void free_program(Program *program) {
    free_catalog(program->catalog);
    free(program);
}

/**
 * Function that asks the user for the path to the dataset folder.
 * If the user doesn't enter anything, the default path is used.
 * Keeps asking until a valid path is entered.
 */
void program_ask_for_dataset_path(Program *program) {
    gboolean loaded = FALSE;
    while (!loaded) {
        char *input = readline("Please enter the path to the dataset folder (default: datasets/data-regular): ");

        if (IS_EMPTY(input)) {
            free(input);
            input = g_strdup("datasets/data-regular");
        }

        loaded = program_load_dataset(program, input);
        free(input);
    }
}

/**
 * Function that runs a query and prints the output to the terminal (paginated if big enough).
 */
void run_query_and_print_output(Catalog *catalog, char *query) {
    GPtrArray *output = g_ptr_array_new_with_free_func(free);
    OutputWriter *writer = create_array_of_semicolon_strings_output_writer(output);
    parse_and_run_query(catalog, writer, query);
    print_content(output);
    close_output_writer(writer);
}

/**
 * Function that runs a query and saves the output to the file according to the given query number.
 */
void run_query_and_save_in_output_file(Catalog *catalog, char *query, int query_number) {
    create_output_folder_if_not_exists();
    FILE *output_file = create_command_output_file(query_number);

    OutputWriter *writer = create_semicolon_file_output_writer(output_file);

    BENCHMARK_START(query_benchmark);
    parse_and_run_query(catalog, writer, query);
    BENCHMARK_LOG("'%s' resolved in %lfs\n", query, g_timer_elapsed(query_benchmark, NULL));

    close_output_writer(writer);

    fclose(output_file);
}

/**
 * Function that executes a program command.
 * If the input is a number, it will execute it as a query and the output will be printed to the terminal (paginated if needed).
 */
void program_execute_command(Program *program, char *input) {
    char **args = g_strsplit(input, " ", 0);
    int arg_size = (int) g_strv_length(args);

    if (arg_size == 0) return;

    const ProgramCommand *program_command = get_program_command(args[0]);

    if (program_command != NULL) {
        run_program_command(program_command, program, args, arg_size);
    } else if (isdigit(args[0][0])) {
        run_query_and_print_output(program->catalog, input);
    } else {
        LOG_WARNING_VA("Invalid command '%s'", args[0]);
    }
}

/**
 * Function that asks the user for a command and executes it.
 * Also saves the command in the history.
 */
void program_ask_for_command(Program *program) {
    char *input = readline(TERMINAL_GREEN "> " TERMINAL_RESET);

    program_execute_command(program, input);

    add_history(input);
    free(input);
}

int start_program(Program *program, GPtrArray *program_args) {
    if (program_args->len >= 2) {
        char *dataset_folder_path = g_ptr_array_index(program_args, 0);
        char *queries_file_path = g_ptr_array_index(program_args, 1);

        if (!program_load_dataset(program, dataset_folder_path))
            return EXIT_FAILURE;
        if (!program_run_queries_from_file(program, queries_file_path))
            return EXIT_FAILURE;
    } else {
        rl_bind_key('\t', rl_complete);
        using_history();

        program_ask_for_dataset_path(program);
        log_info("\nType " TERMINAL_YELLOW_BOLD "help " TERMINAL_RESET "to list all commands\n\n");
        while (program->state != PROGRAM_STATE_EXITING) {
            program_ask_for_command(program);
        }
    }

    return EXIT_SUCCESS;
}

gboolean program_load_dataset(Program *program, char *dataset_folder_path) {
    if (!catalog_load_csv_dataset(program->catalog, dataset_folder_path))
        return FALSE;

    char *lazy_loading_value_string = get_program_flag_value(program->flags, "lazy-loading", "true");
    if (strcmp(lazy_loading_value_string, "true") != 0)
        catalog_force_eager_indexing(program->catalog);

    return TRUE;
}

#define BUFFER_SIZE 1024

gboolean program_run_queries_from_file(Program *program, char *input_file_path) {
    FILE *input_file = open_file(input_file_path);
    if (input_file == NULL) {
        return FALSE;
    }

    BENCHMARK_START(input_file_execution_timer);

    int id = 0;

    char line_buffer[BUFFER_SIZE];
    
    while (fgets(line_buffer, BUFFER_SIZE, input_file)) {
        format_input_line(line_buffer);
        if (*line_buffer == '\0' || *line_buffer == '#') continue; // Hashtag to ignore comments

        run_query_and_save_in_output_file(program->catalog, line_buffer, ++id);
    }

    g_timer_stop(input_file_execution_timer);
    BENCHMARK_LOG("%d queries from '%s' executed in %f seconds\n", id, input_file_path, g_timer_elapsed(input_file_execution_timer, NULL));

    fclose(input_file);

    return TRUE;
}
