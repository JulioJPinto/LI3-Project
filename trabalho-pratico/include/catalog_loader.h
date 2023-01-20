#pragma once
#ifndef LI3_CATALOG_LOADER_H
#define LI3_CATALOG_LOADER_H

#include <glib.h>

#include "catalog.h"

gboolean catalog_load_dataset(Catalog *catalog, const char *dataset_folder_path);

#endif //LI3_CATALOG_LOADER_H
