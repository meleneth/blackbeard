#ifndef ___netjob_inc
#define ___netjob_inc 1

#include"SDL_types.h"
#include<string>
#include<list>
#include"job.hpp"
#include"newsgroup.hpp"

#define MAX_NETJOB_LINES_PER_SLICE 2000

using std::string;
using std::list;

// This handles a multi-line response generic network job

class NetJob : public Job {
    public:
        NetJob();
        ~NetJob();
        virtual void process(void);
        virtual void process_line(string line);
        virtual void finish(void);

        list<string> net_cmds;
        NewsGroup *group;
    private:
    protected:
};

#endif
