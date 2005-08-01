#include "newsgrouplistscreen.hpp"
#include"keydefs.hpp"
#include"session.hpp"
#include"jobqueue.hpp"
#include"newsgrouplistretrieverjob.hpp"
#include"headersforgroupjob.hpp"
#include<ncurses.h>
#include<sstream>
#include<vector>

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
                    if(g)
                        g->is_subscribed ^= 1;
                    return 0;
                case 'p':
                    g = (NewsGroup *) scroll_list->get_selected_item();
                    jobqueue->add_text_job(new HeadersForGroupJob(g));    
                    return 0;
                case 'g':
                    jobqueue->add_text_job(new NewsGroupListRetrieverJob());    
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

}

void NewsGroupListScreen::render_scrollable_line(Uint32 yindex, Uint32 x, Uint32 width, void *newsgroup)
{
    NewsGroup *g = (NewsGroup *) newsgroup;
    stringstream buf;

    buf << g->name << "     " << g->first_article_number << "-" << g->last_article_number
        << ", " << g->last_article_number - g->first_article_number << " possible"; 
    if(g->is_subscribed)
        attron(A_BOLD);
    mvaddnstr(yindex,  x, buf.str().c_str(), -1);
    attroff(A_BOLD);
}

