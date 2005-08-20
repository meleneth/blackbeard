#ifndef ___newsgrouplistretrieverjob_inc
#define ___newsgrouplistretrieverjob_inc 1

#include"netjob.hpp"
#include"SDL_types.h"
#include"stringpattern.hpp"

class NewsGroupListRetrieverJob : public NetJob {
    public:
        NewsGroupListRetrieverJob();
        virtual ~NewsGroupListRetrieverJob();

        virtual void   process_line(string line);
        virtual string status_line();

        StringPattern *breaker;
        Uint8 sent_list_command;

    private:
    protected:
};

#endif
