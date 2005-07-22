#ifndef ___postsetdetailscreen_inc
#define ___postsetdetailscreen_inc 1

#include"SDL_types.h"
#include"screen.hpp"
#include"post_set.hpp"
#include"newsgroup.hpp"
#include"scrollablelist.hpp"
#include<string>
#include<vector>

class PostSetDetailScreen : public Screen {
    public:
        PostSetDetailScreen();
        virtual ~PostSetDetailScreen();

        virtual void render(void);
        virtual int handle_input(int key);
        virtual void render_help(void);
        void render_current_postset(PostSet *set, Uint32 postset_no, Uint32 num_postsets, Uint32 lowest_line);
        virtual void handle_selection(void *postset);
        virtual void render_scrollable_line(Uint32 y, Uint32 x, Uint32 width, void *ptr);
        virtual Uint32 search_match(string search, void *ptr);
    
        ScrollableList<PostFile> *scroll_list;

        PostSet *current_postset;
        NewsGroup *newsgroup;
        Uint32 postset_index;
    private:
    protected:
};

#endif
