#ifndef ___newsgrouplistscreen_inc
#define ___newsgrouplistscreen_inc 1

#include"SDL_types.h"

#include"screen.hpp"

class NewsGroupListScreen : public Screen {
    public:
        NewsGroupListScreen();
        ~NewsGroupListScreen();

        virtual void render(void);
        virtual int handle_input(int key);

    private:
    protected:
};

#endif
