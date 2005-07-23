#include "jobqueue.hpp"
#include"globals.hpp"

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

Job *JobQueue::get_next_postset_job(void)
{
    if(postset_jobs.size()){
        Job *job = *postset_jobs.begin();
	PostsetJob* postsetjob = (PostsetJob*)job;
	if (postsetjob->pieces_left_to_download() <= 0) {
	    postset_jobs.pop_front();
	    return NULL;
	}
        return postsetjob->get_next_job();
    }
    return NULL;
}

void JobQueue::add_decoder_job(Job *job)
{
    decoder_jobs.push_back(job);
}

void JobQueue::add_postset_job(Job *job)
{
    postset_jobs.push_back(job);
}
