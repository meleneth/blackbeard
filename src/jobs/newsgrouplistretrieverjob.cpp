#include "newsgrouplistretrieverjob.hpp"
#include"nntpserver.hpp"

NewsGroupListRetrieverJob::NewsGroupListRetrieverJob()
{
}

NewsGroupListRetrieverJob::~NewsGroupListRetrieverJob()
{
}

string NewsGroupListRetrieverJob::status_line(void)
{
    return status;
}

void NewsGroupListRetrieverJob::process(void *ptr)
{
    status = "Retrieving group list";
    NNTPServer *srv = (NNTPServer *)ptr;
    srv->send_command(LIST);
    
}

