#ifndef ___newsgrouplistretrieverjob_inc
#define ___newsgrouplistretrieverjob_inc 1

#include"job.hpp"
#include"SDL_types.h"

class NewsGroupListRetrieverJob : public Job {
    public:
        NewsGroupListRetrieverJob();
        virtual ~NewsGroupListRetrieverJob();

        virtual void process(void *connection);
        string status_line();

    private:
    protected:
};

#endif
