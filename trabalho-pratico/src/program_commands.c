#include "program_commands.h"

#include "program.h"
#include "logger.h"
#include "output_writer.h"
#include "terminal_controller.h"
#include "file_util.h"

typedef struct ProgramCommand {
    char *name;
    char *description;
    void (*function)(Program *program, char **args, int arg_size);
} ProgramCommand;

void program_run_queries_from_file_command(Program *program, char **args, int arg_size) {
    if (arg_size < 2) {
        LOG_WARNING("Usage: 'file <file_path>'");
        return;
    }

    char *input_file_path = args[1];

    if (!program_run_queries_from_file(program, input_file_path)) {
        LOG_WARNING_VA("Failed to run queries from file '%s'", input_file_path);
    }
}

void program_run_help_command(Program *program, char **args, int arg_size);

void program_exit_command(Program *program, char **args, int arg_size) {
    (void) args;
    (void) arg_size;

    program_set_state(program, PROGRAM_STATE_EXITING);
}

void program_restart_command(Program *program, char **args, int arg_size) {
    (void) args;
    (void) arg_size;

    program_set_state(program, PROGRAM_STATE_EXITING);
    program_set_should_exit(program, FALSE);
}

void program_clear_command(Program *program, char **args, int arg_size) {
    (void) args;
    (void) arg_size;
    (void) program;

    int r = system("clear");
    (void) r;
}

void program_list_output_command(Program *program, char **args, int arg_size) {
    (void) args;
    (void) arg_size;
    (void) program;

    int r = system("ls Resultados/");
    (void) r;
}

void program_cat_files_command(Program *program, char **args, int arg_size) {
    (void) program;

    if (arg_size < 2) {
        LOG_WARNING("Usage: 'cat <file|output_file|output_file_id>'");
        return;
    }

    char *input_file_path = args[1];

    OutputWriter *output = create_array_of_strings_output_writer();

    FILE *input_file = fopen(input_file_path, "r");
    if (input_file == NULL) {
        char *file_path = g_strdup_printf(OUTPUT_FOLDER PATH_SEPARATOR "%s", input_file_path);
        input_file = fopen(file_path, "r");
        free(file_path);
    }
    if (input_file == NULL) {
        char *file_path = g_strdup_printf(OUTPUT_FOLDER PATH_SEPARATOR OUTPUT_FILE_NAME_S, input_file_path);
        input_file = fopen(file_path, "r");
        free(file_path);
    }
    if (input_file == NULL) {
        LOG_WARNING_VA("Could not find file, output file or output file id with name '%s'", input_file_path);
        return;
    }

    char line_buffer[1024];
    while (fgets(line_buffer, 1024, input_file) != NULL) {
        write_output_line(output, line_buffer);
    }

    print_content(get_buffer(output));

    close_output_writer(output);
    fclose(input_file);
}

const ProgramCommand program_commands[] = {
        {"cat", "Prints the output from a result or input file", program_cat_files_command},
        {"clear", "Cleans everything in the terminal", program_clear_command},
        {"exit", "Exits the program", program_exit_command},
        {"file", "Runs all the queries from a file", program_run_queries_from_file_command},
        {"help", "Shows this help message", program_run_help_command},
        {"list-output", "Shows the output files available", program_list_output_command},
        {"restart", "Restarts the program", program_restart_command},
};

const int program_commands_size = sizeof(program_commands) / sizeof(ProgramCommand);

void program_run_help_command(Program *program, char **args, int arg_size) {
    (void) program;
    (void) args;
    (void) arg_size;

    log_info(TERMINAL_YELLOW_BOLD "Available commands:\n");
    log_info(TERMINAL_YELLOW "  <query_id> <query>" TERMINAL_RESET " - Runs a query\n");

    for (int i = 0; i < program_commands_size; i++) {
        log_info(TERMINAL_YELLOW "  %s" TERMINAL_RESET " - %s\n", program_commands[i].name, program_commands[i].description);
    }
}

const ProgramCommand *get_program_command(char *command_name) {
    for (int i = 0; i < program_commands_size; i++) {
        if (strcmp(program_commands[i].name, command_name) == 0) {
            return &program_commands[i];
        }
    }

    return NULL;
}

void run_program_command(const ProgramCommand *command, Program *program, char **args, int arg_size) {
    command->function(program, args, arg_size);
}
