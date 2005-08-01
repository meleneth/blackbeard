#ifndef ___headersforgroupjob_inc
#define ___headersforgroupjob_inc 1

#include"SDL_types.h"
#include"xoverjob.hpp"
#include"job.hpp"
#include"newsgroup.hpp"

#define HEADERS_PER_CHUNK 5000

class Job;
class HeadersForGroupJob : public Job {
    public:
        HeadersForGroupJob(NewsGroup *group);
        virtual ~HeadersForGroupJob();

        XoverJob *get_next_job(void);
        virtual void process(void *connection);

        NewsGroup *group;
        Uint32 lower_id;
        Uint32 upper_id;
        Uint32 finished;
    private:
    protected:
};

#endif
