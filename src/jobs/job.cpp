#include "job.hpp"

Job::Job()
{
    priority = 50;
    is_finished = 0;
    srv = NULL;

    _status = "Default status for Job";
}

Job::~Job()
{
}

void Job::process()
{
}

string Job::status_line()
{
   return _status;
}

