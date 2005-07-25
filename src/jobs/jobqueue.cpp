#include "jobqueue.hpp"

JobQueue::JobQueue() 
{
}
    
JobQueue::~JobQueue() 
{
}

Job *JobQueue::get_next_decoder_job(void)
{
    if(jobs.size()){
        Job *job = *jobs.begin();
        jobs.pop_front();
        return job;
    }
    return NULL;
}

void JobQueue::add_decoder_job(Job *job)
{
    jobs.push_back(job);
}
