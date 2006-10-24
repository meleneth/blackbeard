#include "post_set_splitter.hpp"
#include "console.hpp"

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

void PostSetSplitter::log_info(void)
{
    console->log("No info for generic PostSetSplitter");
}
