#pragma once
#ifndef LI3_PROGRAM_COMMANDS_H
#define LI3_PROGRAM_COMMANDS_H

#include "program.h"

/**
 * Struct that represents a program command that can be executed in interactive mode.
 */
typedef struct ProgramCommand ProgramCommand;

/**
 * Returns the program command with the given name.
 * Returns NULL if no command with the given name exists.
 * The commands are registered in the `program_commands.c` file.
 */
const ProgramCommand *get_program_command(char *command_name);

/**
 * Runs the given program command with the given arguments.
 */
void run_program_command(const ProgramCommand *command, Program *program, char **args, int arg_size);

#endif //LI3_PROGRAM_COMMANDS_H
