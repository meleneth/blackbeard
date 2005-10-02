#include "post_set_splitter_dynamicmatch.hpp"
#include "console.hpp"
#include "strutil.hpp"

#include <string>

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
    return 0 == simple_x(h1->subject).compare(simple_x(h2->subject));
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
            eatme[i] = '\t';
        }
    }

    size_t s = eatme.find("\t\t");
    while(s < string::npos){
        eatme.replace(s, 2, "\t");
        s=eatme.find("\t\t");
    }

    Uint32 first_quote = eatme.find("\"", 0);
    if(first_quote != string::npos){
        Uint32 second_quote = eatme.find("\"", first_quote + 1);
        if(second_quote != string::npos){
            eatme.replace(first_quote +1, second_quote - first_quote -1, "\t");
        }
    }

    return eatme;

}


PSDMSubMatch::PSDMSubMatch(MessageHeader *h1, MessageHeader *h2)
{
    is_active = 0;
    filename_index     = -1;
    piece_no_index     = -1;
    max_piece_no_index = -1;
    file_no_index      = -1;
    max_file_no_index  = -1;

    vector<string> header_pieces;
    string subject = simple_x(h1->subject);
    console->log(subject);
    Tokenize(subject, header_pieces, "\t");

    Uint32 max_no = header_pieces.size();
    pattern = new StringPattern(max_no + 2);
    for(Uint32 i=0; i<max_no; ++i){
        string p = header_pieces[i];
        pattern->add_breaker(p);
        if(p[p.length() -1] == '"'){
            console->log("Found filename..");
            filename_index = i+1;
        }
    }
}

PSDMSubMatch::~PSDMSubMatch()
{
}

void PSDMSubMatch::process_header(MessageHeader *header)
{
    if(pattern->match(header->subject)){
        if(filename_index != -1){
            console->log("Handling result for '" + pattern->results[filename_index] + "'");
        } else {
            console->log("Bliss would be being able to handle :: " + header->subject);
            console->log("Especially since I knew what to do");
        }
    }
}
