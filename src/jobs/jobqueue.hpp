#ifndef ___jobqueue_inc
#define ___jobqueue_inc 1

#include "SDL_types.h"
#include "job.hpp"
#include <vector>

using std::vector;

class JobQueue {
    public:
        JobQueue();
        virtual ~JobQueue(); 

        virtual Job *get_next_job(void);

        void add_job(Job *job);

        virtual void process_jobs(void);

        void finish(Job *job);

        vector<Job *> jobs;
        vector<Job *> active_jobs;
    private:
    protected:
};

extern JobQueue *jobqueue;

#endif
