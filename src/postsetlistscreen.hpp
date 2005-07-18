#ifndef ___postsetlistscreen_inc
#define ___postsetlistscreen_inc 1

#include"SDL_types.h"
#include"screen.hpp"
#include"newsgroup.hpp"

class PostSetListScreen : public Screen{
    public:
        PostSetListScreen();
        virtual ~PostSetListScreen();

        virtual void render(void);
        virtual int handle_input(int key);
        
        Uint32 postset_index;
        Uint32 scroll_index;

        NewsGroup *newsgroup;
    private:
    protected:
};

#endif
