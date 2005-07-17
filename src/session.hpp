#ifndef ___session_inc
#define ___session_inc 1

#include"SDL_types.h"
#include"screen.hpp"

class Session : public Screen{
    public:
        Session();
        ~Session();

        virtual void render(void);
        virtual int handle_input(int key);

        Widget *current_screen;
        
    private:
    protected:
};

#endif
