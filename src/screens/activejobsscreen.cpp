#include"activejobsscreen.hpp"
#include"jobqueue.hpp"
#include"netcentral.hpp"
#include"console.hpp"

ActiveJobsScreen::ActiveJobsScreen()
{
    active_list = new ScrollableList<Job>;
    active_list->screen = this;
    widgets.push_back(active_list);

    queued_list = new ScrollableList<Job>;
    queued_list->screen = this;
    widgets.push_back(queued_list);
}

ActiveJobsScreen::~ActiveJobsScreen()
{
}

void ActiveJobsScreen::handle_selection(Uint32 index)
{
}

void ActiveJobsScreen::render_scrollable_line(Uint32 yindex, Uint32 x, Uint32 width, void *job)
{
    Job *current_job = (Job *) job;
    if(current_job){
        console->log(current_job->status_line().c_str());
        mvaddnstr(yindex, x, current_job->status_line().c_str(), -1);
    }
}

void ActiveJobsScreen::render(void)
{
    active_list->height = (height - 3)/2;
    active_list->width = width -1;
    active_list->ypos = ypos + 1;
    active_list->all_items = jobqueue->jobs;

    queued_list->height = (height - 3)/2;
    queued_list->width = width -1;
    queued_list->ypos = active_list->height + ypos + 2;
    queued_list->all_items = netcentral->jobs;

    Screen::render();
    console->draw_box(0, 0, width-1, (height-1)/2);

    mvaddnstr(ypos, xpos + 1, "ActiveJobsScreen::render", -1);
}

void ActiveJobsScreen::render_help(void)
{
    Uint32 yindex = ypos +1;
    
    mvaddnstr(yindex++, xpos + 2, "This screen shows you a list of active jobs....  and no actual real help..... yet", -1);
}

Uint32 ActiveJobsScreen::search_match(string search, void *ptr)
{
    return ((Job *)ptr)->status_line().find(search) != string::npos;
}

int ActiveJobsScreen::handle_input(int key)
{
    static ScrollableList<Job> *ptr;
    if (key == IKEY_TAB) 
    {
        ptr = ptr == active_list ? queued_list : active_list;
    }
    return ptr->handle_input(key) ?  Screen::handle_input(key) : 0;
}
