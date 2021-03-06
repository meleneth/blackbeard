#include "newsgrouplistscreen.hpp"
#include "keydefs.hpp"
#include "session.hpp"
#include "netcentral.hpp"
#include "jobqueue.hpp"
#include "newsgrouplistretrieverjob.hpp"
#include "headersforgroupjob.hpp"
#include "groupupdater.hpp"
#include "console.hpp"
#ifdef __WIN32__
#include<curses.h>
#else
#include<ncurses.h>
#endif
#include<sstream>
#include<vector>
#include<algorithm>

using std::stringstream;
using std::string;
using std::vector;

NewsGroupListScreen::NewsGroupListScreen()
{
    scroll_list = new ScrollableList<NewsGroup>;
    scroll_list->screen = this;
    widgets.push_back(scroll_list);
}

NewsGroupListScreen::~NewsGroupListScreen()
{
}

Uint32 NewsGroupListScreen::search_match(string search, void *newsgroup)
{
    return ((NewsGroup *)newsgroup)->name.find(search) != string::npos;
}

void NewsGroupListScreen::handle_selection(Uint32 index)
{
    session->switch_postset_list((NewsGroup *)scroll_list->get_selected_item());
}

int NewsGroupListScreen::handle_input(int key)
{
    NewsGroup *g;
    if(scroll_list->handle_input(key)){
        if(Screen::handle_input(key)){
            switch(key){
                case 's':
                    g = (NewsGroup *) scroll_list->get_selected_item();
                    if(g) {
                        if(g->is_subscribed) {
                            g->unsubscribe();
                        } else { 
                            g->subscribe();
                        }
                    }
                    console->log("Toggling subscribed on " + g->name);
                    return 0;
                case 'p':
                    g = (NewsGroup *) scroll_list->get_selected_item();
                    netcentral->add_job(new HeadersForGroupJob(g));    
                    console->log("Getting headers for group " + g->name);
                    return 0;
                case 'g':
                    netcentral->add_job(new NewsGroupListRetrieverJob());    
                    console->log("Getting newsgroup list..");
                    return 0;
                case 'u':
                    g = (NewsGroup *) scroll_list->get_selected_item();
                    if(g){
                        netcentral->add_job(new GroupUpdater(g));
                        console->log("Made new group updater for ");
                    }
                    return 0;
                case 'S':
                    scroll_list->sort();
                    scroll_list->pos_index = 0;
                    return 0;
                default:
                ;
            }
        }
    }
    return 1;
}

void NewsGroupListScreen::render(void)
{

    scroll_list->height = height - 2;
    scroll_list->width = width -1;
    scroll_list->ypos = ypos + 1;
    

    if(scroll_list->known_size != newsgroups.size()){
        scroll_list->all_items = newsgroups;
    }

    Screen::render();

    mvaddnstr(ypos, xpos + 1, "NewsGroupListScreen::render", -1);
    
}

void NewsGroupListScreen::render_help(void)
{
    Uint32 yindex = ypos +1;
    
    mvaddnstr(yindex++, xpos + 2, "This screen shows you a list of newsgroups.", -1);
    yindex++;
    mvaddnstr(yindex++, xpos + 2, "[enter] to see a list of PostSets for the selected group", -1);
    mvaddnstr(yindex++, xpos + 2, "[g] fetch list of newsgroups from server", -1);
    mvaddnstr(yindex++, xpos + 2, "[p] grab postsets for selected group", -1);
    mvaddnstr(yindex++, xpos + 2, "[s] subscribe/unsubscribe to group", -1);
    mvaddnstr(yindex++, xpos + 2, "[u] update newsgroup with newer headers", -1);

}

void NewsGroupListScreen::render_scrollable_line(Uint32 yindex, Uint32 x, Uint32 width, void *newsgroup)
{
    NewsGroup *g = (NewsGroup *) newsgroup;
    if(g->is_subscribed)
        attron(A_BOLD);
    string news_stats = g->status_string();
    mvaddnstr(yindex,  x, (char*)news_stats.c_str(), -1);
    if(g->is_subscribed)
        attroff(A_BOLD);
}

