#ifndef ___shutdownscreen_inc
#define ___shutdownscreen_inc 1

#include"screen.hpp"
#include"SDL_types.h"

class ShutDownScreen :public Screen{
    public:
        ShutDownScreen();
        ~ShutDownScreen();

        virtual void render(void);

    private:
    protected:
};

#endif
