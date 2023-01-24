#include "catalog_loader.h"

#include "file_util.h"
#include "parser.h"
#include "benchmark.h"

#define RED_BOLD "\033[1;31m"
#define RED "\033[0;31m"
#define YELLOW_BOLD "\033[1;33m"
#define YELLOW "\033[0;33m"
#define GREEN_BOLD "\033[1;32m"
#define GREEN "\033[0;32m"
#define STANDARD "\033[0;37m"

gboolean catalog_load_dataset(Catalog *catalog, const char *dataset_folder_path) {
    FILE *users_file = open_file_folder(dataset_folder_path, "users.csv");
    FILE *drivers_file = open_file_folder(dataset_folder_path, "drivers.csv");
    FILE *rides_file = open_file_folder(dataset_folder_path, "rides.csv");

    if (users_file == NULL || drivers_file == NULL || rides_file == NULL) {
        return FALSE;
    }

    BENCHMARK_START(load_timer);
    read_file(users_file, parse_and_register_user, catalog);
    BENCHMARK_END(load_timer, YELLOW_BOLD "Load users time:        %f seconds\n");

    g_timer_start(load_timer);
    read_file(drivers_file, parse_and_register_driver, catalog);
    BENCHMARK_END(load_timer, YELLOW_BOLD "Load drivers time:      %f seconds\n");

    g_timer_start(load_timer);
    read_file(rides_file, parse_and_register_ride, catalog);
    BENCHMARK_END(load_timer, YELLOW_BOLD "Load rides time:        %f seconds\n");

    fclose(users_file);
    fclose(drivers_file);
    fclose(rides_file);

    return TRUE;
}
