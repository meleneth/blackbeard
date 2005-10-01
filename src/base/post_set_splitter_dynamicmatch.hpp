#ifndef ___post_set_splitter_dynamicmatch_inc
#define ___post_set_splitter_dynamicmatch_inc 1

#include"SDL_types.h"

#include "post_set_splitter.hpp"

class PostSetSplitterDynamicMatch : public PostSetSplitter {
    public:
        PostSetSplitterDynamicMatch(NewsGroup *group);
        ~PostSetSplitterDynamicMatch();

        virtual void process_header(MessageHeader *header);

    private:
    protected:
};

#endif
