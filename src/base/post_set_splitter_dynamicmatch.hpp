#ifndef ___post_set_splitter_dynamicmatch_inc
#define ___post_set_splitter_dynamicmatch_inc 1

#include"SDL_types.h"

#include "post_set_splitter.hpp"
#include "messageheader.hpp"
#include "stringpattern.hpp"
#include <vector>
#include <string>

using std::vector;
using std::string;

class PSDMSubMatch {
    public:
        PSDMSubMatch(NewsGroup *group, MessageHeader *h1, MessageHeader *h2);
        ~PSDMSubMatch();

        void process_header(MessageHeader *header);
        string subject(MessageHeader *header);
        PostSet *get_postset(MessageHeader *header);
        string super_simple(string eatme);

        PostSet *postset;
        StringPattern *pattern;
        string posted_by;

        Sint32 filename_index;
        Sint32 piece_no_index;
        Sint32 max_piece_no_index;
        Sint32 file_no_index;
        Sint32 max_file_no_index;
        NewsGroup *group;
        Uint32 last_seen_article_no;

        Uint32 is_active;
    private:
    protected:
};

class PostSetSplitterDynamicMatch : public PostSetSplitter {
    public:
        PostSetSplitterDynamicMatch(NewsGroup *group);
        ~PostSetSplitterDynamicMatch();

        virtual void process_header(MessageHeader *header);
        Uint32 is_close(MessageHeader *h1, MessageHeader *h2);
        void reprocess_unprocessed(void);

        list <MessageHeader *> unprocessed;
        list <PSDMSubMatch *> active;
    private:
    protected:
};

string simple_x(string eatme);


#endif
