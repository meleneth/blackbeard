#include "job.hpp"
#include "jobparent.hpp"

JobParent::JobParent()
{
}

JobParent::~JobParent()
{
}

void JobParent::notify_child_finished(Job *finished)
{
    vector<Job *> still_running;

    Uint32 max_no = children.size();
    for(Uint32 i=0; i<max_no; ++i){
        if(!children[i]->is_finished){
            still_running.push_back(children[i]);
        }
    }
    children = still_running;
}

void JobParent::add_child_job(Job *job)
{
    job->parent = this;
    children.push_back(job);
}

