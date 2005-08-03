#ifndef ___activejobsscreen_inc
#define ___activejobsscreen_inc 1

#include"scrollablelist.hpp"
#include"SDL_types.h"
#include"screen.hpp"
#include"job.hpp"
#include<string>

using std::string;

class ActiveJobsScreen : public Screen {
    public:
        ActiveJobsScreen();
        virtual ~ActiveJobsScreen();

        virtual void render(void);
        virtual void render_help(void);
        virtual int handle_input(int key);
        virtual void handle_selection(Uint32 index);
        virtual void render_scrollable_line(Uint32 y, Uint32 x, Uint32 width, void *ptr);
        virtual Uint32 search_match(string search, void *ptr);

        ScrollableList<Job> *active_list;
        ScrollableList<Job> *queued_list;
    private:
    protected:
};

#endif
