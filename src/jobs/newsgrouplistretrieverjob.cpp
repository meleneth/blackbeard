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
    return "no status for newsgroup retriever";
}

void NewsGroupListRetrieverJob::process(void *ptr)
{
    NNTPServer *srv = (NNTPServer *)ptr;
    
}

