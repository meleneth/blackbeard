#ifndef ___postsetlistscreen_inc
#define ___postsetlistscreen_inc 1

#include"SDL_types.h"
#include"screen.hpp"
#include"newsgroup.hpp"
#include<string>
#include<vector>

class PostSetListScreen : public Screen{
    public:
        PostSetListScreen();
        virtual ~PostSetListScreen();

        virtual void render(void);
        virtual void render_help(void);
        virtual int handle_input(int key);
        void refine_search(void);
        
        
        vector< PostSet * >my_postsets;
        string search_string;
        Uint32 is_searching;
        Uint32 postset_index;
        Uint32 scroll_index;
        Uint32 known_size;

        NewsGroup *newsgroup;
    private:
    protected:
};

#endif
