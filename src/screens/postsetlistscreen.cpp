#include "postsetlistscreen.hpp"
#include"keydefs.hpp"
#include"console.hpp"
#include"session.hpp"
#include"jobqueue.hpp"
#include"netcentral.hpp"
#include"postsetjob.hpp"
#include"headersforgroupjob.hpp"
#ifdef __WIN32__
#include<curses.h>
#else
#include<ncurses.h>
#endif
#include<sstream>

using std::stringstream;

PostSetListScreen::PostSetListScreen()
{
    scroll_list = new ScrollableList<PostSet>;
    scroll_list->screen = this;
    widgets.push_back(scroll_list);
    newsgroup = NULL;
}

PostSetListScreen::~PostSetListScreen()
{
}

void PostSetListScreen::handle_selection(Uint32 index)
{
    NewsGroup *group = newsgroup;
    session->switch_postset_detail(group,  index);
}

void PostSetListScreen::render_scrollable_line(Uint32 yindex, Uint32 x, Uint32 width, void *postset)
{
    stringstream buf;
    string str;
    
    PostSet *set = (PostSet *) postset;
    
    mvaddnstr(yindex, x, (char*)set->subject.c_str(), -1);
    
    buf << "[" << set->num_files() << "]";
    str = buf.str();
    mvaddnstr(yindex,  x + width - 3 - str.length(), (char*)str.c_str(), -1);
}

void PostSetListScreen::render(void)
{
    scroll_list->height = height - 2;
    scroll_list->width = width -1;
    scroll_list->ypos = ypos + 1;
    
    string statusline = "PostSetListScreen::render";
    if(newsgroup){
        if(scroll_list->known_size != newsgroup->postsets.size()){
            scroll_list->all_items = newsgroup->postsets;
        }
        statusline += " + " + newsgroup->name;
    }
    Screen::render();
    mvaddnstr(ypos, xpos + 1, (char*)statusline.c_str(), -1);
}

void PostSetListScreen::render_help(void)
{
    Uint32 yindex = ypos +1;
    
    mvaddnstr(yindex++, xpos + 2, "This screen shows you a list of post sets. hit enter to see a list of files.", -1);
    mvaddnstr(yindex++, xpos + 2, "d will download the selected postset", -1);
    mvaddnstr(yindex++, xpos + 2, "p will grab headers for the selected postset (use if 0/0)", -1);
}

Uint32 PostSetListScreen::search_match(string search, void *ptr)
{
    return ((PostSet *)ptr)->subject.find(search) != string::npos;
}

int PostSetListScreen::handle_input(int key)
{
    if(scroll_list->handle_input(key)){
        if(key == 'd'){
            PostSet *s = (PostSet *) scroll_list->get_selected_item();
            metajobs->add_job(new PostsetJob(s));
            console->log("Queued download job for " + s->subject);
            return 0;
        }
        if(key == 'p'){
            PostSet *s = (PostSet *) scroll_list->get_selected_item();
            netcentral->add_job(new HeadersForGroupJob(s->group, s->_min_article_no, s->_max_article_no));
            console->log("Queued header retrieval for postset " + s->subject);
            return 0;
        }
        if (key == 'S') {
            scroll_list->sort();
            scroll_list->pos_index = 0;
        }
    }
    return Screen::handle_input(key);
}


