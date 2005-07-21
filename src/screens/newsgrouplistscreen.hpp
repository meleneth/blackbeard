#ifndef ___newsgrouplistscreen_inc
#define ___newsgrouplistscreen_inc 1

#include"SDL_types.h"

#include"screen.hpp"
#include"newsgroup.hpp"
#include"scrollablelist.hpp"
#include<string>

using std::string;
using std::vector;

class NewsGroupListScreen : public Screen {
    public:
        NewsGroupListScreen();
        ~NewsGroupListScreen();

        virtual void render(void);
        virtual void render_help(void);
        virtual void handle_selection(void *newsgroup);
        virtual int handle_input(int key);
        virtual void render_scrollable_line(Uint32 y, Uint32 x, Uint32 width, void *ptr);
        virtual Uint32 search_match(string search, void *ptr);

        ScrollableList<NewsGroup> *scroll_list;

    private:
    protected:
};

#endif
