#ifndef ___screen_inc
#define ___screen_inc 1

#include"SDL_types.h"

#include"widget.hpp"

#include<vector>
#include<string>

using std::vector;
using std::string;

class Screen : public Widget {
    public:
        Screen();
        virtual ~Screen();

        virtual void render(void);
        virtual void render_help(void);
        virtual int handle_input(int key);
        virtual void handle_selection(Uint32 index);
        virtual void render_scrollable_line(Uint32 y, Uint32 x, Uint32 width, void *ptr);
        virtual Uint32 search_match(string search, void *ptr);
        vector<Widget *> widgets;
        Uint32 help_visible;
    private:
    protected:
};

#endif
