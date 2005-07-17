#ifndef __globals_hpp
#define __globals_hpp 1

#include"console.hpp"
#include"config.hpp"
#include"newsgroup.hpp"
#include"post_set.hpp"
#include"post_file.hpp"
#include"jobqueue.hpp"
#include"session.hpp"

#include<list>

using std::list;

extern Config *config;
extern NewsGroup *newsgroup;
extern list<NewsGroup *> newsgroups;
//extern PostSet *current_postset;
extern PostFile *current_postfile;
extern JobQueue *jobqueue;
extern void shut_down(void);
extern Session *session;

#endif
