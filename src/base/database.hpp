#ifndef ___database_inc
#define ___database_inc 1

#include "SDL_types.h"
#include "newsgroup.hpp"
#include "post_set.hpp"
#include "post_file.hpp"

void save_db_data();
void restore_db_data();

void setup_newsgroup_tables(sqlite3 *db);
void setup_postset_tables(sqlite3 *db);
void run_queries(sqlite3 *db, vector<string> queries);

void save_subscribed_groups_to_db(sqlite3* db);
void save_postsets_to_db(sqlite3 *db, NewsGroup *group);
void save_postfiles(sqlite3* db, PostSet *set); 
void save_ids_to_db(sqlite3* db, PostFile *file);

void restore_newsgroups_from_db(sqlite3 *db);
void restore_postsets_from_db(sqlite3 *db, NewsGroup *group);
void restore_postfiles_from_db(sqlite3 *db, PostSet *set);
void restore_ids_from_db(sqlite3 *db, PostFile *file);
void restore_ids_from_db(PostFile *file);

void remove_postset_info_from_db(PostSet *set);

sqlite3 *db_for_newsgroup(NewsGroup *group);

#endif
