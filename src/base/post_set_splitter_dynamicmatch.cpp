#include "post_set_splitter_dynamicmatch.hpp"
#include "console.hpp"
#include "strutil.hpp"

#include <string>

PostSetSplitterDynamicMatch::PostSetSplitterDynamicMatch(NewsGroup *group):PostSetSplitter(group)
{
    
}

PostSetSplitterDynamicMatch::~PostSetSplitterDynamicMatch()
{
    list<PSDMSubMatch *>::iterator i;
    for(i = active.begin(); i != active.end(); ++i){
        delete *i;
    }
    active.clear();
}

void PostSetSplitterDynamicMatch::process_header(MessageHeader *header)
{
    list<PSDMSubMatch *>::iterator i;
    list<MessageHeader *>::iterator j;

    for(i = active.begin(); i!=active.end(); ++i){
        PSDMSubMatch *match = *i;
        if (0 == match->posted_by.compare(header->posted_by)){
            if(match->pattern->match(header->subject)){
                match->process_header(header);
                match->last_seen_article_no = header->article_no;
                delete header;
                return;
            }
        }
    }
    
    unprocessed.push_back(header);

    for(j = unprocessed.begin(); j!=unprocessed.end(); ++j){
        MessageHeader *unproc = *j;
        if(is_close(unproc, header)){
            active.push_back(new PSDMSubMatch(group, header, unproc));
            reprocess_unprocessed();
            return;
        }
    }
}

Uint32 PostSetSplitterDynamicMatch::is_close(MessageHeader *h1, MessageHeader *h2)
{
    return 0 == simple_x(h1->subject).compare(simple_x(h2->subject));
}

void PostSetSplitterDynamicMatch::reprocess_unprocessed(void)
{
    list<MessageHeader *>::iterator i;
    list<PSDMSubMatch *>::iterator j;

    for(i = unprocessed.begin(); i != unprocessed.end() ; ++i)
    {
        MessageHeader *h = *i;

        for(j = active.begin(); j != active.end(); ++j)
        {
            if(h) {
                if((*j)->pattern->match(h->subject)){
                    (*j)->process_header(h);
                    list<MessageHeader *>::iterator p = i;
                    --i;
                    unprocessed.erase(p);
                    delete h;
                    h = NULL;
                }
            }
        }
        if(h){
            h->process_count++;
            if(h->process_count > 1000){
                list<MessageHeader *>::iterator p = i;
                --i;
                unprocessed.erase(p);
                delete h;
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

    eatme = replace_all_substrings(eatme, "\t\t", "\t");

    size_t first_quote = eatme.find("\"", 0);
    if(first_quote != string::npos){
        size_t second_quote = eatme.find("\"", first_quote + 1);
        if(second_quote != string::npos){
            eatme.replace(first_quote +1, second_quote - first_quote -1, "\t");
        }
    }

    return eatme;

}


PSDMSubMatch::PSDMSubMatch(NewsGroup *group, MessageHeader *h1, MessageHeader *h2)
{
    is_active = 0;
    filename_index     = -1;
    piece_no_index     = -1;
    max_piece_no_index = -1;
    file_no_index      = -1;
    max_file_no_index  = -1;
    last_seen_article_no = 0;
    this->group = group;
    postset = NULL;
    posted_by = h1->posted_by;

    vector<string> header_pieces;
    string subject = simple_x(h1->subject);
    Tokenize(subject, header_pieces, "\t");

    Uint32 max_no = header_pieces.size();
    pattern = new StringPattern(max_no + 2);
    for(Uint32 i=0; i<max_no; ++i){
        string p = header_pieces[i];
        pattern->add_breaker(p);
        if(p[p.length() -1] == '"'){
            filename_index = i+1;
        }
    }
}

PSDMSubMatch::~PSDMSubMatch()
{
    delete pattern;
}

void PSDMSubMatch::process_header(MessageHeader *header)
{
    if(pattern->match(header->subject)){
        if(group->last_article_number < header->article_no){
            group->last_article_number = header->article_no;
        }
        if(group->first_article_number == 0)
            group->first_article_number = header->article_no;

        if(filename_index != -1){
//            console->log("Handling result for '" + pattern->results[filename_index] + "'");
            postset = get_postset(header);
            postset->dirty = 1;
            PostFile *file = postset->file(pattern->results[filename_index]);
            file->saw_message_id(header->article_no, header->msg_id, header->num_bytes);
        } else {
            /* FIXME
            console->log("no file name found error");
            console->log("Bliss would be being able to handle :: " + header->subject);
            console->log("Especially since I knew what to do");
            */
        }
    }
}

PostSet *PSDMSubMatch::get_postset(MessageHeader *header)
{
    if(postset)
        return postset;
    string s = header->subject;
    string simple = super_simple(s);
    Uint32 max_no = group->postsets.size();
    for(Uint32 i=0; i<max_no; ++i){
        if(simple.compare(group->postsets[i]->subject) ==0){
            return group->postsets[i];
        }
    }

    PostSet *p = group->postset_for_subject(simple);
    p->poster = header->posted_by;
    return p;
}

string PSDMSubMatch::super_simple(string eatme)
{
    Uint32 max_no = eatme.length();
    for(Uint32 i=0; i<max_no; ++i){
        if((eatme[i] >= '0') && (eatme[i] <= '9')){
            eatme[i] = ' ';
        }
    }

    eatme = replace_all_substrings(eatme, "  ", " ");

    size_t first_quote = eatme.find("\"", 0);
    if(first_quote != string::npos){
        size_t second_quote = eatme.find("\"", first_quote + 1);
        if(second_quote != string::npos){
            eatme.replace(first_quote +1, second_quote - first_quote -1, " ");
        }
    }

    eatme = replace_all_substrings(eatme, "( / )", " ");
    eatme = replace_all_substrings(eatme, " - \" \"", " ");
    eatme = replace_all_substrings(eatme, " \" \"", " ");
    eatme = replace_all_substrings(eatme, " yEnc", " ");
    eatme = replace_all_substrings(eatme, "[ / ]", " ");
    eatme = replace_all_substrings(eatme, "  ", " ");

    return eatme;
}

string PSDMSubMatch::subject(MessageHeader *header)
{
    string sub = header->subject;
    size_t pos = sub.find("\"", 0);
    if(pos != string::npos){
        sub = sub.substr(0, pos);
    }
    return sub;
}
