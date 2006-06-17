#ifndef ___database_inc
#define ___database_inc 1

#include "SDL_types.h"
#include "newsgroup.hpp"
#include "post_set.hpp"
#include "post_file.hpp"

void save_db_data();
void restore_db_data();

void save_newsgroup(NewsGroup *group);
void save_subscribed_groups();
void save_postsets(NewsGroup *group);
void save_postfiles(PostSet *set); 

void restore_newsgroups();
void restore_postsets(NewsGroup *group);
void restore_postfiles(PostSet *set);

void remove_postset_file(PostSet *set);

Uint32 file_exists(string filename);

#endif
