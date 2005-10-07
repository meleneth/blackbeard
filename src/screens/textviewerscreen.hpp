#ifndef ___textviewerscreen_inc
#define ___textviewerscreen_inc 1

#include"scrollablelist.hpp"
#include"SDL_types.h"
#include"screen.hpp"
#include"job.hpp"
#include<string>

using std::string;

class TextViewerScreen : public Screen {
    public:
        TextViewerScreen();
        virtual ~TextViewerScreen();

        virtual void render(void);
        virtual void render_help(void);
        virtual int handle_input(int key);
        virtual void handle_selection(Uint32 index);
        virtual void render_scrollable_line(Uint32 y, Uint32 x, Uint32 width, void *ptr);
        virtual Uint32 search_match(string search, void *ptr);
        virtual void set_file(PostFile *post_file);

        ScrollableList<NewsGroupPost> *text_list;
        PostFile *file;
    private:
    protected:
};

#endif
