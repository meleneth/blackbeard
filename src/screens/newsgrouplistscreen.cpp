#include "newsgrouplistscreen.hpp"
#include"globals.hpp"
#include"keydefs.hpp"
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

void NewsGroupListScreen::handle_selection(void *newsgroup)
{
    session->switch_postset_list((NewsGroup *)newsgroup);
}

int NewsGroupListScreen::handle_input(int key)
{
    return scroll_list->handle_input(key) ?  Screen::handle_input(key) : 0;
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
    
    mvaddnstr(yindex++, xpos + 2, "This screen shows you a list of newsgroups. hit enter to see a list of PostSets.", -1);

}

void NewsGroupListScreen::render_scrollable_line(Uint32 yindex, Uint32 x, Uint32 width, void *newsgroup)
{
     mvaddnstr(yindex,  x, ((NewsGroup *)newsgroup)->name.c_str(), -1);
}

