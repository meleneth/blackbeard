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
        void add_decoder_job(Job *job);

        list<Job *> jobs;
    private:
    protected:
};

extern JobQueue *jobqueue;

#endif
