#include "job.hpp"
#include "file_handle.hpp"

Job::Job()
{
    priority = 50;
    is_finished = 0;
    srv = NULL;

    _status = "Default status for Job";
    job_status_filename = "";
    job_type = MISC;
    parent = NULL;
}

Job::~Job()
{
    close_finished_files();
}

void Job::process()
{
}

string Job::status_line()
{
   return _status;
}

bool Job::has_more_fruit()
{
    return 0;
}

void Job::shake_tree()
{
}

void Job::notify_child_finished(Job *job)
{
    vector<Job *> still_running;

    Uint32 max_no = children.size();
    for(Uint32 i=0; i<max_no; ++i){
        still_running.push_back(children[i]);
    }
    children = still_running;
}

void Job::add_child_job(Job *job)
{
    job->parent = this;
    children.push_back(job);
}

void Job::finish(void)
{
    is_finished = 1;
    if(parent){
        parent->notify_child_finished(this);
    }
}

bool Job::compare(const Job* a, const Job* b)
{
    return (a->priority < b->priority);
}

