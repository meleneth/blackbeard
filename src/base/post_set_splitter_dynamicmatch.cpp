#include "post_set_splitter_dynamicmatch.hpp"
#include "console.hpp"

PostSetSplitterDynamicMatch::PostSetSplitterDynamicMatch(NewsGroup *group):PostSetSplitter(group)
{
    
}

PostSetSplitterDynamicMatch::~PostSetSplitterDynamicMatch()
{
}

void PostSetSplitterDynamicMatch::process_header(MessageHeader *header)
{
    Uint32 max_no = active.size();
    for(Uint32 i=0; i<max_no; ++i){
        if (0 == active[i]->posted_by.compare(header->posted_by)){
            if(active[i]->pattern->match(header->subject)){
                active[i]->process_header(header);
                return;
            }
        }
    }
    
    max_no = unprocessed.size();
    for(Uint32 i=0; i<max_no; ++i){
        if(is_close(unprocessed[i], header)){
            active.push_back(new PSDMSubMatch(header, unprocessed[i]));
            reprocess_unprocessed();
            return;
        }
    }

    unprocessed.push_back(header);
}

Uint32 PostSetSplitterDynamicMatch::is_close(MessageHeader *h1, MessageHeader *h2)
{
    console->log("No TV and no Beer make Homer something something");
    return 1;
}

void PostSetSplitterDynamicMatch::reprocess_unprocessed(void)
{
    Uint32 max_no = unprocessed.size();
    Uint32 max_active_no = active.size();
    for(Uint32 j=0; j<max_active_no; ++j){
        for(Uint32 i=0; i<max_no; ++i){
            if(active[j]->pattern->match(unprocessed[i]->subject)){
                active[j]->process_header(unprocessed[i]);
            }
        }
    }
}

string simple_x(string eatme)
{
    Uint32 max_no = eatme.length();
    for(Uint32 i=0; i<max_no; ++i){
        if((eatme[i] >= '0') && (eatme[i] <= '9')){
            eatme[i] = 'X';
        }
    }

    return eatme;

}


PSDMSubMatch::PSDMSubMatch(MessageHeader *h1, MessageHeader *h2)
{
    
}

PSDMSubMatch::~PSDMSubMatch()
{
}

void PSDMSubMatch::process_header(MessageHeader *header)
{
}
