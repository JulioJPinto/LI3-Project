#pragma once
#ifndef LI3_PROGRAM_COMMANDS_H
#define LI3_PROGRAM_COMMANDS_H

#include "program.h"

typedef struct ProgramCommand ProgramCommand;

const ProgramCommand *get_program_command(char *command_name);

void run_program_command(const ProgramCommand *command, Program *program, char **args, int arg_size);

#endif //LI3_PROGRAM_COMMANDS_H
