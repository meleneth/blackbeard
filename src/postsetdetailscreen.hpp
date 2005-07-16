#ifndef ___postsetdetailscreen_inc
#define ___postsetdetailscreen_inc 1

#include"SDL_types.h"
#include"screen.hpp"
#include"post_set.hpp"

class PostSetDetailScreen : public Screen {
    public:
        PostSetDetailScreen();
        virtual ~PostSetDetailScreen();

        virtual void render(void);
        virtual int handle_input(int key);
        void render_current_postset(PostSet *set, Uint32 postset_no, Uint32 num_postsets, Uint32 lowest_line);

        PostSet *current_postset;
        Uint32 postset_index;
    private:
    protected:
};

#endif
