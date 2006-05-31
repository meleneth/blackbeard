#ifndef ___mnzb_inc
#define ___mnzb_inc 1

#include "SDL_types.h"

#include "post_set.hpp"

class mNZB {
    public:
        mNZB();
        ~mNZB();

        void save_postset(PostSet *set);
        void load_postset(PostSet *set);
        PostSet *set;

    private:
    protected:
};

#endif
