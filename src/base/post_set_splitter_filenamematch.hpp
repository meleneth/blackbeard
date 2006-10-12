#ifndef ___post_set_splitter_filenamematch_inc
#define ___post_set_splitter_filenamematch_inc 1

#include "SDL_types.h"
#include "post_set_splitter.hpp"
#include "stringpattern.hpp"

class PostSetSplitterFilenameMatch : public PostSetSplitter {
    public:
        PostSetSplitterFilenameMatch(NewsGroup *group);
        ~PostSetSplitterFilenameMatch();

        virtual void process_header(MessageHeader *header);

        StringPattern *pattern;
        
    private:
    protected:
};

#endif
