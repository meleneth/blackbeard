#ifndef ___downloadedpostsetsscreen_inc
#define ___downloadedpostsetsscreen_inc 1

#include"SDL_types.h"
#include"scrollablelist.hpp"
#include"screen.hpp"

class DownloadedPostSetsScreen : public Screen {
    public:
        DownloadedPostSetsScreen();
        ~DownloadedPostSetsScreen();

        virtual void render(void);
        virtual void render_help(void);
        virtual int handle_input(int key);
        virtual void handle_selection(Uint32 index);
        virtual void render_scrollable_line(Uint32 y, Uint32 x, Uint32 width, void *ptr);
        virtual Uint32 search_match(string search, void *ptr);

        ScrollableList<PostSet> *postsets_list;

    private:
    protected:
};

#endif
