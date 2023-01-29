#pragma once
#ifndef LI3_CATALOG_USER_H
#define LI3_CATALOG_USER_H

#include <glib.h>

#include "user.h"

/**
 * Struct that represents a catalog of users.
 */
typedef struct CatalogUser CatalogUser;

/**
 * Creates a new empty CatalogUser allocated in the heap memory.
 */
CatalogUser *create_catalog_user(void);

/**
 * Frees the memory allocated for the catalog.
 */
void free_catalog_user(CatalogUser *catalog_user);

/**
 * Registers a user in the catalog.
 */
void catalog_user_register_user(CatalogUser *catalog_user, User *user);

/**
 * Returns the user with the given id.
 * This id is generated when the user is added to the catalog.
 */
User *catalog_user_get_user_by_user_id(CatalogUser *catalog_user, int user_id);

/**
 * Returns the user with the given username.
 */
User *catalog_user_get_user_by_username(CatalogUser *catalog_user, char *username);

/**
 * Forces the catalog to index all the users.
 */
void catalog_user_force_eager_indexing(CatalogUser *catalog_user);

/**
 * Retrieves the top n users with the most distance travelled (using `compare_users_by_total_distance`).
 * The result is stored in the given GPtrArray.
 */
int catalog_user_get_top_n_users(CatalogUser *catalog_user, int n, GPtrArray *result);

#endif //LI3_CATALOG_USER_H
