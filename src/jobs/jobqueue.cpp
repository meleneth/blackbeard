#include "jobqueue.hpp"
#include "postsetjob.hpp"
#include "headersforgroupjob.hpp"
#include "console.hpp"

JobQueue::JobQueue() 
{
}
    
JobQueue::~JobQueue() 
{
    Uint32 max_no = active_jobs.size();
    for(Uint32 i=0; i<max_no; ++i){
        delete active_jobs[i];
    }
    active_jobs.clear();

    max_no = jobs.size();
    for(Uint32 i=0; i<max_no; ++i){
        delete jobs[i];
    }
    jobs.clear();
}

void JobQueue::process_jobs(void)
{
    Uint32 j;
    Job *job;
    Uint32 max_num = active_jobs.size();
    
    if(0 == max_num){
        Job *job = get_next_job();
        if(job) {
            active_jobs.push_back(job);
            max_num = 1;
        }
    }
    for(j=0; j<max_num; ++j){
        job = active_jobs[j];
        job->process();
        if (job->is_finished){
            finish_job(job);
            Job *new_job = get_next_job();
            if(new_job){
                active_jobs[j] = new_job;
            }else{
                active_jobs.erase(active_jobs.begin() + j);
                max_num = active_jobs.size();
                --j;
            }
            delete job;
        }
    }
}

Job *JobQueue::get_next_job(void)
{
    if(jobs.size()){
        Job * job = *jobs.begin();
        jobs.erase(jobs.begin());
        return job;
    }
    return NULL;
}


void JobQueue::add_job(Job *job)
{
    jobs.push_back(job);
}

void JobQueue::finish_job(Job *job)
{
}


