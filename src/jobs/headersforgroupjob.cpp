#include "headersforgroupjob.hpp"
#include"nntpserver.hpp"

HeadersForGroupJob::HeadersForGroupJob(NewsGroup *group)
{
    this->group = group;
    finished = 0;
}

HeadersForGroupJob::~HeadersForGroupJob()
{
}

XoverJob *HeadersForGroupJob::get_next_job(void)
{
    if(finished)
        return NULL;

    Uint32 upid;
    if ((lower_id + HEADERS_PER_CHUNK) > upper_id ){
        upid = upper_id;
        finished = 1;
    }else{
        upid = lower_id + HEADERS_PER_CHUNK;
        lower_id += HEADERS_PER_CHUNK;
    }

    return new XoverJob(group, lower_id, upid);
}

void HeadersForGroupJob::process(void *connection)
{
    NNTPServer *srv = (NNTPServer *) connection;
}
