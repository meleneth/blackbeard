#ifndef ___jobqueue_inc
#define ___jobqueue_inc 1

#include "SDL_types.h"
#include "job.hpp"
#include <vector>

using std::vector;

class JobQueue {
    public:
        JobQueue();
        ~JobQueue(); 

        Job *get_next_decoder_job(void);
        Job *get_next_text_job(void);

        void add_decoder_job(Job *job);
        void add_text_job(Job *job);

        void process_jobs(void);

        void finish(Job *job);

        vector<Job *> decoder_jobs;
        vector<Job *> text_jobs;

        vector<Job *> active_jobs;
    private:
    protected:
};

extern JobQueue *jobqueue;

#endif
