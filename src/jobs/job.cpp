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

