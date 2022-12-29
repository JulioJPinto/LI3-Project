#pragma once
#ifndef LI3_CATALOG_USER_H
#define LI3_CATALOG_USER_H

#include <glib.h>

#include "user.h"

typedef struct CatalogUser CatalogUser;

CatalogUser *create_catalog_user(void);

void free_catalog_user(CatalogUser *catalog_user);

void catalog_user_register_user(CatalogUser *catalog_user, User *user);

User *catalog_user_get_user(CatalogUser *catalog_user, char *username);

void catalog_user_notify_stop_registering(CatalogUser *catalog_user);

int catalog_user_get_top_n_users(CatalogUser *catalog_user, int n, GPtrArray *result);

#endif //LI3_CATALOG_USER_H
