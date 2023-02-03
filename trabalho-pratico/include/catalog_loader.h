#pragma once
#ifndef LI3_CATALOG_LOADER_H
#define LI3_CATALOG_LOADER_H

#include <glib.h>

#include "catalog.h"

/**
 * Loads the dataset from the given folder path.
 * Returns true if the dataset was loaded successfully.
 * This function registers all the users, drivers, rides and other data needed in the catalog.
 */
gboolean catalog_load_csv_dataset(Catalog *catalog, const char *dataset_folder_path);

#endif //LI3_CATALOG_LOADER_H
