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
        virtual int handle_input(int key);

        vector<Widget *> widgets;
    private:
    protected:
};

#endif
