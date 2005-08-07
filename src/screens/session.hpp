#ifndef ___session_inc
#define ___session_inc 1

#include"SDL_types.h"
#include"screen.hpp"
#include"newsgroup.hpp"
#include"post_set.hpp"

#include"newsgrouplistscreen.hpp"
#include"postsetlistscreen.hpp"
#include"postsetdetailscreen.hpp"
#include"activejobsscreen.hpp"
#include"shutdownscreen.hpp"

class Session : public Screen{
    public:
        Session();
        virtual ~Session();

        virtual void render(void);
        virtual int handle_input(int key);

        void switch_postset_detail(NewsGroup *newsgroup, Uint32 postset_no);
        void switch_postset_list(NewsGroup *newsgroup);
        void switch_shutdown_screen(void);

        NewsGroupListScreen *newsgrouplist; 
        PostSetListScreen *postsetlist;
        PostSetDetailScreen *postsetdetail;
        ActiveJobsScreen *activejobs; 
        Widget *current_screen;

    private:
    protected:
};

extern Session *session;

#endif
