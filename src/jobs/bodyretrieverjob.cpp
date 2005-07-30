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

