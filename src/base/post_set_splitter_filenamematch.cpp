#include "post_set_splitter_filenamematch.hpp"
#include "console.hpp"

PostSetSplitterFilenameMatch::PostSetSplitterFilenameMatch(NewsGroup *group):PostSetSplitter(group)
{
    pattern = new StringPattern(3);
    pattern->add_breaker("\"");
    pattern->add_breaker("\"");
}

PostSetSplitterFilenameMatch::~PostSetSplitterFilenameMatch()
{
}

void PostSetSplitterFilenameMatch::process_header(MessageHeader *header)
{
    if(pattern->match(header->subject)) {
        console->log("Filename: " + pattern->results[1] + " -*- " + header->subject);
    } else {
        console->log("Filename NOT FOUND - " + header->subject);
    }
}

