#include "downloadedpostsetsscreen.hpp"
#include "session.hpp"

DownloadedPostSetsScreen::DownloadedPostSetsScreen()
{
    postsets_list = new ScrollableList<PostSet>;
    postsets_list->screen = this;
    widgets.push_back(postsets_list);
}

DownloadedPostSetsScreen::~DownloadedPostSetsScreen()
{
}

void DownloadedPostSetsScreen::handle_selection(Uint32 index)
{
    PostSet *set = (PostSet *) postsets_list->get_selected_item();
    session->switch_postset_detail(set->group, set->group->postset_index(set));
}

void DownloadedPostSetsScreen::render_scrollable_line(Uint32 yindex, Uint32 x, Uint32 width, void *postset)
{
    stringstream buf;
    string str;
    
    PostSet *set = (PostSet *) postset;
    
    mvaddnstr(yindex, x, (char*)set->subject.c_str(), -1);
    
    buf << "[" << set->num_files << "/" << set->max_num_files() << "]";
    str = buf.str();
    mvaddnstr(yindex,  x + width - 3 - str.length(), (char*)str.c_str(), -1);
}

void DownloadedPostSetsScreen::render(void)
{
    postsets_list->height = height - 2;
    postsets_list->width = width -1;
    postsets_list->ypos = ypos + 1;
    
    string statusline = "DownloadedPostSetsScreen::render";
    postsets_list->all_items = downloaded_postsets;
    Screen::render();
    mvaddnstr(ypos, xpos + 1, (char*)statusline.c_str(), -1);
}

void DownloadedPostSetsScreen::render_help(void)
{
    Uint32 yindex = ypos +1;
    
    mvaddnstr(yindex++, xpos + 2, "This screen shows you a list of post sets. hit enter to see a list of files.", -1);
    mvaddnstr(yindex++, xpos + 2, "d will download the selected postset", -1);
    mvaddnstr(yindex++, xpos + 2, "p will grab headers for the selected postset (use if 0/0)", -1);
}

Uint32 DownloadedPostSetsScreen::search_match(string search, void *ptr)
{
    return ((PostSet *)ptr)->subject.find(search) != string::npos;
}

int DownloadedPostSetsScreen::handle_input(int key)
{
    if(postsets_list->handle_input(key)){
        return 0;
    }
    return Screen::handle_input(key);
}

