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
        Job *job = *text_jobs.begin();
	switch (job->type()) {
	    case 1:
		PostsetJob* postsetjob = (PostsetJob*)job;
		if (postsetjob->pieces_left_to_download() <= 0) {
			text_jobs.pop_front();
			return NULL;
		} else {
			return postsetjob->get_next_job();
		}
		break;
            case 2:
		HeadersForGroupJob* headerjob = (HeadersForGroupJob*)job;
		break;
            case 3:
		XoverJob* xoverjob = (XoverJob*)job;
		break;
            default:
		;
	}
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
