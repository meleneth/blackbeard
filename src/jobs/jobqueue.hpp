#ifndef ___jobqueue_inc
#define ___jobqueue_inc 1

#include "SDL_types.h"
#include "job.hpp"
#include <list>

using std::list;

class JobQueue {
    public:
        JobQueue();
        ~JobQueue(); 

        Job *get_next_decoder_job(void);
        Job *get_next_postset_job(void);

        void add_decoder_job(Job *job);
        void add_postset_job(Job *job);

        list<Job *> decoder_jobs;
        list<Job *> postset_jobs;
    private:
    protected:
};

extern JobQueue *jobqueue;

#endif
