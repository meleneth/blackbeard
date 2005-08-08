#include "job.hpp"

Job::Job()
{
    priority = 50;
    is_finished = 0;
    srv = NULL;
}

Job::~Job()
{
}

void Job::process()
{
}

string Job::status_line()
{
   return "Default status for Job";
}

