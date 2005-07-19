#ifndef __globals_hpp
#define __globals_hpp 1

#include"console.hpp"
#include"config.hpp"
#include"newsgroup.hpp"
#include"jobqueue.hpp"
#include"session.hpp"

#include<list>
#include<vector>

using std::list;
using std::vector;

extern Config *config;
extern vector<NewsGroup *> newsgroups;
extern JobQueue *jobqueue;
extern Session *session;

extern void shut_down(void);

#endif
