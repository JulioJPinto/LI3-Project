#pragma once
#ifndef LI3_PROGRAM_FLAGS_H
#define LI3_PROGRAM_FLAGS_H

#include <glib.h>

typedef struct ProgramFlags ProgramFlags;

ProgramFlags *retrieve_program_flags(GPtrArray *program_args);

char *get_program_flag_value(ProgramFlags *program_flags, char *flag, char *default_value);

void free_program_flags(ProgramFlags *program_flags);

#endif //LI3_PROGRAM_FLAGS_H
