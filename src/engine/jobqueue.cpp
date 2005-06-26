#include "jobqueue.hpp"
#include"globals.hpp"

JobQueue::JobQueue() 
{
}
    
JobQueue::~JobQueue() 
{
}

Decoder *JobQueue::get_next_decoder_job(void)
{
    if(decoder_jobs.size()){
        Decoder *job = *decoder_jobs.begin();
        decoder_jobs.pop_front();
        return job;
    }
    return NULL;
}

void JobQueue::add_decoder_job(Decoder *job)
{
    decoder_jobs.push_back(job);
}

