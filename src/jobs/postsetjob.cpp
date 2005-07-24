#include "postsetjob.hpp"
#include"globals.hpp"

PostsetJob::PostsetJob(PostSet* post_set)
{
    postset = post_set;
}

PostsetJob::~PostsetJob()
{
}

Job* PostsetJob::get_next_job()
{
    Job* job = new Job();
    return job;
}

Uint32 PostsetJob::pieces_left_to_download()
{
    return (postset->num_finished_pieces < postset->num_pieces);
}

