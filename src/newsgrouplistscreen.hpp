#ifndef ___newsgrouplistscreen_inc
#define ___newsgrouplistscreen_inc 1

#include"SDL_types.h"

#include"screen.hpp"
#include"newsgroup.hpp"

class NewsGroupListScreen : public Screen {
    public:
        NewsGroupListScreen();
        ~NewsGroupListScreen();

        virtual void render(void);
        virtual int handle_input(int key);

        Uint32 ng_index;
        Uint32 scroll_index;
        NewsGroup *newsgroup;
    private:
    protected:
};

#endif
