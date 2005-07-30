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
