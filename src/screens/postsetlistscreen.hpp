#ifndef ___postsetlistscreen_inc
#define ___postsetlistscreen_inc 1

#include"SDL_types.h"
#include"screen.hpp"
#include"newsgroup.hpp"
#include"scrollablelist.hpp"
#include<string>
#include<vector>

class PostSetListScreen : public Screen {
    public:
        PostSetListScreen();
        virtual ~PostSetListScreen();

        virtual void render(void);
        virtual void render_help(void);
        virtual int handle_input(int key);
        virtual void handle_selection(void *postset);
        virtual void render_scrollable_line(Uint32 y, Uint32 x, Uint32 width, void *ptr);
        virtual Uint32 search_match(string search, void *ptr);
        void refine_search(void);
    
        ScrollableList<PostSet> *scroll_list;
        NewsGroup *newsgroup;
    private:
    protected:
};

#endif
