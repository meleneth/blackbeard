#ifndef ___database_inc
#define ___database_inc 1

#include "SDL_types.h"
#include "newsgroup.hpp"
#include "post_set.hpp"
#include "post_file.hpp"

void save_db_data();
void restore_db_data();

void save_postsets_to_db(sqlite3 *db, NewsGroup *group);
void save_subscribed_groups_to_db(sqlite3* db);
void setup_newsgroup_tables(sqlite3 *db);
void save_postfiles(sqlite3* db, PostSet *set); 
void save_ids_to_db(sqlite3* db, PostFile *file);

void restore_newsgroups_from_db(sqlite3 *db);
void restore_postsets_from_db(sqlite3 *db, NewsGroup *group, Uint32 group_index);
void restore_postfiles_from_db(sqlite3 *db, PostSet *set, Uint32 postset_index);
void restore_ids_from_db(sqlite3 *db, PostFile *file, Uint32 postfile_index);

#endif
