#include "postsetlistscreen.hpp"
#include"keydefs.hpp"
#include"console.hpp"
#include"session.hpp"
#include"jobqueue.hpp"
#include"postsetjob.hpp"
#include<ncurses.h>
#include<sstream>

using std::stringstream;

PostSetListScreen::PostSetListScreen()
{
    scroll_list = new ScrollableList<PostSet>;
    scroll_list->screen = this;
    widgets.push_back(scroll_list);
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
    set->recalculate_piece_info();
    
    mvaddnstr(yindex, x, set->subject.c_str(), -1);
    
    buf << "[" << set->num_files << "/" << set->max_num_files << "]";
    str = buf.str();
    mvaddnstr(yindex,  x + width - 3 - str.length(), str.c_str(), -1);
}

void PostSetListScreen::render(void)
{
    NewsGroup *group = newsgroup;

    scroll_list->height = height - 2;
    scroll_list->width = width -1;
    scroll_list->ypos = ypos + 1;
    
    if(group){
        if(scroll_list->known_size != group->postsets.size()){
            scroll_list->all_items = group->postsets;
        }
    }
    Screen::render();

    mvaddnstr(ypos, xpos + 1, "PostSetListScreen::render", -1);
}

void PostSetListScreen::render_help(void)
{
    Uint32 yindex = ypos +1;
    
    mvaddnstr(yindex++, xpos + 2, "This screen shows you a list of post sets. hit enter to see a list of files.", -1);
}

Uint32 PostSetListScreen::search_match(string search, void *ptr)
{
    return ((PostSet *)ptr)->subject.find(search) != string::npos;
}

int PostSetListScreen::handle_input(int key)
{
    if(scroll_list->handle_input(key)){
        if(key == 'd'){
            console->log("Caught keyboard input 'd'");
            jobqueue->add_job(new PostsetJob((PostSet *) scroll_list->get_selected_item()));
            return 0;
        }
    }
    return Screen::handle_input(key);
}
