#ifndef ___post_set_splitter_inc
#define ___post_set_splitter_inc 1

#include"SDL_types.h"
#include"newsgroup.hpp"
#include"messageheader.hpp"

class MessageHeader;
class PostSetSplitter {
    public:
        PostSetSplitter(NewsGroup *group);
        virtual ~PostSetSplitter();

        NewsGroup *group;

        virtual void process_header(MessageHeader *header);

    private:
    protected:
};

#endif
