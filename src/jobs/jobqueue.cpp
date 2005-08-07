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

void JobQueue::process_jobs(void)
{
    Uint32 j;
    Job *job;
    Uint32 max_num = jobs.size();
    
    for(j=0; j<max_num; ++j){
        job = jobs[j];
        job->process();
        if (job->is_finished){
            finish(job);
            max_num = jobs.size();
            --j;
        }
    }
}

Job *JobQueue::get_next_job(void)
{
    /*if(jobs.size()){
        vector<Job *>::iterator j;
        Job * job;
        
        j = jobs.begin();
        job = *j;
        decoder_jobs.erase(j);
        active_jobs.push_back(job);
        return job;
    }*/
    return NULL;
}


void JobQueue::add_job(Job *job)
{
    jobs.push_back(job);
}

void JobQueue::finish(Job *job)
{
    vector<Job *>::iterator j;
    
    for(j = jobs.begin(); j != jobs.end(); ++j){
        if((*j) == job){
            jobs.erase(j);
            return;
        }
    }
}

