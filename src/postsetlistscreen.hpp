#ifndef ___postsetlistscreen_inc
#define ___postsetlistscreen_inc 1

#include"SDL_types.h"
#include"screen.hpp"

class PostSetListScreen : public Screen{
    public:
        PostSetListScreen();
        virtual ~PostSetListScreen();

        virtual void render(void);
        virtual int handle_input(int key);
        
        Uint32 postset_index;

    private:
    protected:
};

#endif
