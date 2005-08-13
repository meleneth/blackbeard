#include "bodyretrieverjob.hpp"
#include"nntpserver.hpp"

BodyRetrieverJob::BodyRetrieverJob()
{
}

BodyRetrieverJob::~BodyRetrieverJob()
{
}

void BodyRetrieverJob::process(void *connection)
{
    NNTPServer *srv = (NNTPServer *) connection;
}

string BodyRetrieverJob::status_line(void)
{
    return "Default status for BodyRetrieverJob";
}
