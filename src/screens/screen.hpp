#ifndef ___screen_inc
#define ___screen_inc 1

#include"SDL_types.h"

#include"widget.hpp"

#include<vector>

using std::vector;

class Screen : public Widget {
    public:
        Screen();
        virtual ~Screen();

        virtual void render(void);
        virtual void render_help(void);
        virtual int handle_input(int key);

        vector<Widget *> widgets;
        Uint32 help_visible;
    private:
    protected:
};

#endif
