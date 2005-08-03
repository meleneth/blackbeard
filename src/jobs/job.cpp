#include "job.hpp"

Job::Job()
{
    priority = 50;
    is_finished = 0;
}

Job::~Job()
{
}

void Job::process()
{
}

void Job::process(void *connection)
{
    // do stuff here
}

string Job::status_line()
{
   return "Default status for Job";
}

pthread_mutex_t job_lock = PTHREAD_MUTEX_INITIALIZER;

void lock_jobs(void)
{
    pthread_mutex_lock(&job_lock);
}

void unlock_jobs(void)
{
    pthread_mutex_unlock(&job_lock);
}


