#include "jobqueue.hpp"
#include "postsetjob.hpp"
#include "headersforgroupjob.hpp"
#include "xoverjob.hpp"

JobQueue::JobQueue() 
{
}
    
JobQueue::~JobQueue() 
{
}

Job *JobQueue::get_next_decoder_job(void)
{
    if(decoder_jobs.size()){
        Job *job = *decoder_jobs.begin();
        decoder_jobs.pop_front();
        return job;
    }
    return NULL;
}

Job *JobQueue::get_next_text_job(void)
{
    if(text_jobs.size()){
        return *text_jobs.begin();
    }
    return NULL;
}

void JobQueue::add_decoder_job(Job *job)
{
    decoder_jobs.push_back(job);
}

void JobQueue::add_text_job(Job *job)
{
    text_jobs.push_back(job);
}

void JobQueue::finish(Job *job)
{
    active_jobs.remove(job);
    dead_jobs.push_front(job);
}

