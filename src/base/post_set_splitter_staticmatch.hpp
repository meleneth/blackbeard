#ifndef ___post_set_splitter_staticmatch_inc
#define ___post_set_splitter_staticmatch_inc 1

#include"SDL_types.h"
#include"post_set_splitter.hpp"

class PostSetSplitterStaticMatch : public PostSetSplitter {
    public:
        PostSetSplitterStaticMatch(NewsGroup *group);
        ~PostSetSplitterStaticMatch();

        virtual void process_header(MessageHeader *header);

        list< StringPattern * > yenc_subject_patterns;
        list< StringPattern * > uu_subject_patterns;
    
    private:
    protected:
};

#endif
