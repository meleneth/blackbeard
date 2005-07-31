#include"activejobsscreen.hpp"
#include"jobqueue.hpp"
#include"console.hpp"

ActiveJobsScreen::ActiveJobsScreen()
{
    scroll_list = new ScrollableList<Job>;
    scroll_list->screen = this;
    widgets.push_back(scroll_list);
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
    console->log(current_job->status_line().c_str());
    mvaddnstr(yindex, x, current_job->status_line().c_str(), -1);
}

void ActiveJobsScreen::render(void)
{
    scroll_list->height = height - 3;
    scroll_list->width = width -1;
    scroll_list->ypos = ypos + 1;
    
    scroll_list->all_items = jobqueue->text_jobs;
    Screen::render();

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
    return scroll_list->handle_input(key) ?  Screen::handle_input(key) : 0;
}
