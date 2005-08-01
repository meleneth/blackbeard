#include "xoverjob.hpp"
#include"nntpserver.hpp"

XoverJob::XoverJob(NewsGroup *group, Uint32 lower, Uint32 upper)
{
    this->group = group;
    lower_id = lower;
    upper_id = upper;
}

XoverJob::~XoverJob()
{
}

void XoverJob::process(void *connection)
{
    NNTPServer *srv = (NNTPServer *) connection;
}
