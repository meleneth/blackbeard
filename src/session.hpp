#ifndef ___session_inc
#define ___session_inc 1

#include"SDL_types.h"
#include"screen.hpp"
#include"newsgroup.hpp"

class Session : public Screen{
    public:
        Session();
        ~Session();

        virtual void render(void);
        virtual int handle_input(int key);

        void switch_screen(void);
        void switch_postset_detail(NewsGroup *newsgroup, int postset_no);

        Uint32 current_screen_id;
        
    private:
    protected:
};

#endif
