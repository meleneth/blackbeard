#ifndef ___post_set_splitter_dynamicmatch_inc
#define ___post_set_splitter_dynamicmatch_inc 1

#include"SDL_types.h"

#include "post_set_splitter.hpp"
#include "messageheader.hpp"
#include "stringpattern.hpp"
#include <vector>

using std::vector;

class PSDMSubMatch {
    public:
        PSDMSubMatch(MessageHeader *h1, MessageHeader *h2);
        ~PSDMSubMatch();

        void process_header(MessageHeader *header);
        PostSet *postset;
        StringPattern *pattern;
        string posted_by;
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

        vector <MessageHeader *> unprocessed;
        vector <PSDMSubMatch *> active;
    private:
    protected:
};


#endif
