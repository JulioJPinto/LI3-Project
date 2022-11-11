#include <stdio.h>

#include "catalog.h"
#include "file_util.h"

int main(int argc, char **argv) {
#ifdef DEBUG
    printf("Running in debug mode.\n");
#endif

    if (argc < 2) {
        printf("Usage: %s <dataset_folder_path> <queries_file_path>", argv[0]);
        return 0;
    }

    char *dataset_folder_path = argv[1];
    char *queries_file_path = argv[2];

    FILE *drivers_file = open_file_folder(dataset_folder_path, "drivers.csv");
    FILE *rides_file = open_file_folder(dataset_folder_path, "rides.csv");
    FILE *users_file = open_file_folder(dataset_folder_path, "users.csv");

    Catalog *catalog = create_catalog();

    //    register_ride(catalog, ...)
    //    register_user(catalog, ...)
    //    register_driver(catalog, ...)

    fclose(drivers_file);
    fclose(rides_file);
    fclose(users_file);

    FILE *queries_file = open_file(queries_file_path);

//    char *line_buffer = malloc(1024 * sizeof(char));
//    while (!fgets(line_buffer, 1024, queries_file)) {
//        // execute_query(catalog, line_buffer);
//    }

    free_catalog(catalog);

    fclose(queries_file);

    return 0;
}
