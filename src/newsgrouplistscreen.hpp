#ifndef ___newsgrouplistscreen_inc
#define ___newsgrouplistscreen_inc 1

#include"SDL_types.h"

#include"screen.hpp"
#include"newsgroup.hpp"
#include<string>

using std::string;
using std::vector;

class NewsGroupListScreen : public Screen {
    public:
        NewsGroupListScreen();
        ~NewsGroupListScreen();

        virtual void render(void);
        virtual int handle_input(int key);
        void refine_search(void);

        vector< NewsGroup * >my_groups;
        string search_string;
        Uint32 is_searching;
        Uint32 ng_index;
        Uint32 scroll_index;
        Uint32 known_size;
        NewsGroup *newsgroup;
    private:
    protected:
};

#endif
