#pragma once
#ifndef LI3_PARSER_H
#define LI3_PARSER_H

#include <stdio.h>

#include "driver.h"
#include "user.h"
#include "ride.h"

User *read_users_file(FILE *stream, char *line_buffer, int line_buffer_size);

Driver *read_drivers_file(FILE *stream, char *line_buffer, int line_buffer_size);

Ride *read_rides_file(FILE *stream, char *line_buffer, int line_buffer_size);

#endif //LI3_PARSER_H
