#ifndef ___mnzb_inc
#define ___mnzb_inc 1

#include "SDL_types.h"

#include "post_set.hpp"
#include "post_file.hpp"
#include "xmlnode.hpp"

class mNZB {
    public:
        mNZB();
        ~mNZB();

        void save_postset(PostSet *set);
        void load_postset(PostSet *set);
        string nzb_filename(void);

        XMLNode *postfile_node(PostFile *file); 
        void restore_file(PostSet *set, XMLNode *file_node);
        PostSet *set;

    private:
    protected:
};

#endif
