#include "activenetjobsscreen.hpp"
#include "jobqueue.hpp"
#include "netcentral.hpp"
#include "console.hpp"
#include "postsetjob.hpp"
#include "session.hpp"

ActiveNetJobsScreen::ActiveNetJobsScreen()
{
    active_list = new ScrollableList<Job>;
    active_list->screen = this;
    widgets.push_back(active_list);

    queued_list = new ScrollableList<Job>;
    queued_list->screen = this;
    widgets.push_back(queued_list);
    widget_ptr = active_list;
}

ActiveNetJobsScreen::~ActiveNetJobsScreen()
{
}

void ActiveNetJobsScreen::handle_selection(Uint32 index)
{
    Job *job = widget_ptr->my_items[index];

    if(job->job_type == POSTSET_DOWNLOAD){
        PostsetJob *j = (PostsetJob *)job;
        PostSet *set = j->postset;
        session->switch_postset_detail(set->group, set->group->postset_index(set));
    }
}

void ActiveNetJobsScreen::render_scrollable_line(Uint32 yindex, Uint32 x, Uint32 width, void *job)
{
    Job *current_job = (Job *) job;
    if(current_job){
        //console->log(current_job->status_line().c_str());
        mvaddnstr(yindex, x, (char*)current_job->status_line().c_str(), -1);
    }
}

void ActiveNetJobsScreen::render(void)
{
    active_list->height = (height - 3)/2;
    active_list->width = width -1;
    active_list->ypos = ypos + 1;
    active_list->all_items = netcentral->active_jobs;

    queued_list->height = (height - 3)/2;
    queued_list->width = width -1;
    queued_list->ypos = active_list->height + ypos + 2;
    queued_list->all_items = netcentral->jobs;

    Screen::render();
    console->draw_box(0, 0, width-1, (height-1)/2);

    mvaddnstr(ypos, xpos + 1, "ActiveNetJobsScreen::render", -1);
}

void ActiveNetJobsScreen::render_help(void)
{
    Uint32 yindex = ypos +1;
    
    mvaddnstr(yindex++, xpos + 2, "This screen shows you a list of active jobs....  and no actual real help..... yet", -1);
}

Uint32 ActiveNetJobsScreen::search_match(string search, void *ptr)
{
    return ((Job *)ptr)->status_line().find(search) != string::npos;
}

int ActiveNetJobsScreen::handle_input(int key)
{
    if (key == IKEY_TAB) 
    {
        widget_ptr = widget_ptr == active_list ? queued_list : active_list;
    }
    if(widget_ptr){
        return widget_ptr->handle_input(key) ?  Screen::handle_input(key) : 0;
    } else {
        return Screen::handle_input(key);
    }
}
