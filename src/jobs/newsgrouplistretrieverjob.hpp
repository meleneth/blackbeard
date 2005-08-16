#ifndef ___newsgrouplistretrieverjob_inc
#define ___newsgrouplistretrieverjob_inc 1

#include"job.hpp"
#include"SDL_types.h"
#include"stringpattern.hpp"

class NewsGroupListRetrieverJob : public Job {
    public:
        NewsGroupListRetrieverJob();
        virtual ~NewsGroupListRetrieverJob();

        virtual void process(void *connection);
        virtual string status_line();

        StringPattern *breaker;
        Uint8 sent_list_command;

    private:
    protected:
};

#endif
