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

typedef enum ProgramState {
    PROGRAM_STATE_RUNNING,
    PROGRAM_STATE_WAITING_FOR_DATASET_INPUT,
    PROGRAM_STATE_WAITING_FOR_COMMANDS,
    PROGRAM_STATE_VIEWING_QUERY_RESULT,
    PROGRAM_STATE_EXITING,
} ProgramState;

typedef enum ProgramMode {
    WAITING_FOR_MODE,
    BATCH_MODE,
    RUNNING_IN_ITERATIVE_MODE_TO_FILE,
    INTERACTIVE_MODE,
} ProgramMode;

struct Program {
    ProgramFlags *flags;
    Catalog *catalog;
    ProgramState state;
    ProgramMode mode;
    int current_query_id;

    gboolean should_exit;
};

typedef struct ProgramCommand {
    char *name;
    char *description;
    void (*function)(Program *program, char **args, int arg_size);
} ProgramCommand;

gboolean program_should_exit(Program *program) {
    return program->should_exit;
}

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

void program_reload_command(Program *program, char **args, int arg_size) {
    (void) args;
    (void) arg_size;

    program->state = PROGRAM_STATE_EXITING;
    program->should_exit = FALSE;
}

void program_clear_command(Program *program, char **args, int arg_size) {
    (void) args;
    (void) arg_size;
    (void) program;

    system("clear");
}

const ProgramCommand program_commands[] = {
        {"file", "Runs all the queries from a file", program_run_queries_from_file_command},
        {"clear", "Cleans everything in the terminal", program_clear_command},
        {"reload", "Reloads the program", program_reload_command},
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
    program->should_exit = TRUE;
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
        program->mode = BATCH_MODE;
        char *dataset_folder_path = g_ptr_array_index(program_args, 0);
        char *queries_file_path = g_ptr_array_index(program_args, 1);

        if (!program_load_dataset(program, dataset_folder_path))
            return EXIT_FAILURE;
        if (!program_run_queries_from_file(program, queries_file_path))
            return EXIT_FAILURE;

    } else {
        program->mode = INTERACTIVE_MODE;
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
    catalog_load_dataset(program->catalog, dataset_folder_path);

    char *lazy_loading_value_string = get_program_flag_value(program->flags, "lazy-loading", "true");
    if (strcmp(lazy_loading_value_string, "true") != 0)
        catalog_force_eager_indexing(program->catalog);

    return TRUE;
}

#define page_size 10
#define number_of_prints_per_page_changed 14

void print_page_number(GPtrArray *array, int page_number, int number_of_pages) {
    if(page_number <= 0 || page_number > number_of_pages) return;
    int fst_element = page_size*(page_number - 1);
    for(int i = fst_element; i < array->len && i < page_size*page_number ; i++) {
        fprintf(stdout, "%s", (char *) g_ptr_array_index(array, i));
    }
    readline("=== Press Enter to continue =====================");
}

void run_paging_output(OutputWriter *writer) {
    GPtrArray *array = get_buffer(writer);
    int number_of_lines = array->len;
    if(number_of_lines == 1) {
        fprintf(stdout, "%s", (char *) g_ptr_array_index(array, 0));
        return;
    }

    double number_of_pages = (double) number_of_lines / (double) page_size;
    number_of_pages = ceil(number_of_pages);
    while(1) {
        fprintf(stdout, "=== Number of pages %d =====================\n", (int) number_of_pages);
        char* line = readline("=== Type the page you want or press enter to leave:  " );
        int line_value = strcmp(line, "clear"); 
        int page = atoi(line);
        if((!page || page > number_of_pages) && line_value) return;
        print_page_number(array, page,(int) number_of_pages);
        if(page <= number_of_pages && number_of_pages != 1) fprintf(stdout, "=== Page %d of %d pages ============\n", page, (int) number_of_pages);
        if (!line_value) system("clear");
        free(line);
    }
    
    
}

void run_query_for_terminal(Catalog *catalog, char *query, int query_number) {
    fprintf(stdout, "=== Query number: #%d =====================\n", query_number);

    OutputWriter *writer = create_array_of_strings_output_writer();
    parse_and_run_query(catalog, writer, query);
    run_paging_output(writer);
    close_output_writer(writer);

    fprintf(stdout, "===========================================\n");
}

void run_query_for_output_folder(Catalog *catalog, char *query, int query_number) {
    create_output_folder_if_not_exists();
    FILE *output_file = create_command_output_file(query_number);

    OutputWriter *writer = create_file_output_writer(output_file);
    parse_and_run_query(catalog, writer, query);
    close_output_writer(writer);

    fclose(output_file);
}

void program_run_query(Program *program, char *query) {
    Catalog *catalog = program->catalog;
    int query_number = ++program->current_query_id;

    if (*query == '#') {
        return;
    }

    if (program->mode == INTERACTIVE_MODE) {
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

    program->mode = INTERACTIVE_MODE;

    g_timer_stop(input_file_execution_timer);
    BENCHMARK_LOG("%d queries from %s executed in %f seconds\n", program->current_query_id, input_file_path, g_timer_elapsed(input_file_execution_timer, NULL));
    program->current_query_id = aux_id;

    fclose(input_file);

    return TRUE;
}
