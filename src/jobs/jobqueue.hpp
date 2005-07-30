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
        Job *get_next_text_job(void);

        void add_decoder_job(Job *job);
        void add_text_job(Job *job);

        void finish(Job *job);

        list<Job *> decoder_jobs;
        list<Job *> text_jobs;

        list<Job *> active_jobs;
        list<Job *> dead_jobs;
    private:
    protected:
};

extern JobQueue *jobqueue;

#endif
