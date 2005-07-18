#ifndef ___widget_inc
#define ___widget_inc 1

#include"SDL_types.h"

class Widget {
    public:
        Widget();
        virtual ~Widget();

        virtual void render(void);
        virtual int handle_input(int key);

        Uint32 xpos;
        Uint32 ypos;
        Uint32 width;
        Uint32 height;

    private:
    protected:
};

#endif
