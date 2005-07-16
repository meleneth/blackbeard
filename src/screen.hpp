#ifndef ___screen_inc
#define ___screen_inc 1

#include"SDL_types.h"

#include"widget.hpp"

#include<list>
using std::list;

class Screen : public Widget {
    public:
        Screen();
        virtual ~Screen();

        virtual void render(void);

        list<Widget *> widgets;
    private:
    protected:
};

#endif
