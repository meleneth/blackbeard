#include "jobqueue.hpp"
#include "postsetjob.hpp"
#include "headersforgroupjob.hpp"
#include "xoverjob.hpp"
#include "console.hpp"

JobQueue::JobQueue() 
{
}
    
JobQueue::~JobQueue() 
{
}

Job *JobQueue::get_next_decoder_job(void)
{
    if(decoder_jobs.size()){
        vector<Job *>::iterator j;
        Job * job;
        
        j = decoder_jobs.begin();
        job = *j;
        decoder_jobs.erase(j);
        active_jobs.push_back(job);
        return job;
    }
    return NULL;
}

Job *JobQueue::get_next_text_job(void)
{
    if(text_jobs.size()){
        vector<Job *>::iterator j;
        Job * job;
        
        j = text_jobs.begin();
        job = *j;
        text_jobs.erase(j);
        console->log("i found a job in the queue!!!!");
        return job;
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
    vector<Job *>::iterator j;
    
    for(j = active_jobs.begin(); j != active_jobs.end(); ++j){
        if((*j) == job){
          //  active_jobs.erase(j);
          //  dead_jobs.push_back(job);
        }
    }
}

