#pragma once
#ifndef LI3_FILE_UTIL_H
#define LI3_FILE_UTIL_H

#include <stdio.h>

FILE *open_file_folder(const char *folder_path, const char *file_name);

FILE *open_file(const char *file_name);

#endif //LI3_FILE_UTIL_H
