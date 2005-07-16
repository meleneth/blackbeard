#ifndef ___postsetdetailscreen_inc
#define ___postsetdetailscreen_inc 1

#include"SDL_types.h"
#include"screen.hpp"

class PostSetDetailScreen : public Screen {
    public:
        PostSetDetailScreen();
        virtual ~PostSetDetailScreen();

        virtual void render(void);
        virtual int handle_input(int key);

        Uint32 postset_index;
    private:
    protected:
};

#endif
