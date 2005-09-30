#include "post_set_splitter.hpp"

PostSetSplitter::PostSetSplitter(NewsGroup *group)
{
    this->group = group;
}

PostSetSplitter::~PostSetSplitter()
{
}

void PostSetSplitter::process_header(MessageHeader *header)
{
}

