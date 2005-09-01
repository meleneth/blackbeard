#include"postsetdetailscreen.hpp"
#include"keydefs.hpp"
#include"console.hpp"

#include<sstream>
#include<ncurses.h>

using std::stringstream;

PostSetDetailScreen::PostSetDetailScreen()
{
    scroll_list = new ScrollableList<PostFile>;
    scroll_list->screen = this;
    widgets.push_back(scroll_list);
    newsgroup = NULL;
}

PostSetDetailScreen::~PostSetDetailScreen()
{
}

void PostSetDetailScreen::render_help(void)
{
    Uint32 yindex = ypos +1;
    
    mvaddnstr(yindex++, xpos + 2, "This screen shows you a list of post files for the current post set. hit enter to see magic happen.", -1);
}

void PostSetDetailScreen::render(void)
{
    Screen::render();
    if(help_visible)
        return;
    
    Uint32 num_postsets = 0;
    Uint32 three_quarters = (height * 3) / 4;
    Uint32 current_postset_no = 0;

    scroll_list->width = width -1;
    scroll_list->ypos = ypos + 4;
    scroll_list->height = three_quarters - scroll_list->ypos;

    console->draw_box(0, 0, width-1, three_quarters);
    mvaddnstr(ypos, xpos + 1, "PostSetDetailScreen::render", -1);

    if(newsgroup){
        string groupline = newsgroup->name + " ( " + newsgroup->status + " )";
        mvaddnstr(1, 1, groupline.c_str(), -1);
        
        attron(A_BOLD);
        int max_p = newsgroup->postsets.size();
        PostSet *set = NULL;
        if(max_p){
            set = newsgroup->postsets[current_postset_no];
        }

        if(set){
            scroll_list->all_items = set->files;
            mvaddnstr(2, 1, set->status().c_str(), -1);

            stringstream buf;
            buf << "(" << current_postset_no << "/" << num_postsets << ") postsets";
            string str = buf.str();
            mvaddnstr(3, COLS - (str.length() + 3), str.c_str(), -1);
        }
        attroff(A_BOLD);
    }

    console->render(three_quarters, height-4);
}

void PostSetDetailScreen::render_scrollable_line(Uint32 y, Uint32 x, Uint32 width, void *ptr)
{
    if(ptr){
        string completed_bar;
        completed_bar = ((PostFile *) ptr)->get_bar();
        mvaddnstr(y, 1, ((PostFile *) ptr)->status_string().c_str(), -1);
        mvaddnstr(y, COLS - completed_bar.length() -3, completed_bar.c_str(), -1);
    }
}

void PostSetDetailScreen::render_current_postset(PostSet *set, Uint32 postset_no, Uint32 num_postsets, Uint32 lowest_line)
{
}

int PostSetDetailScreen::handle_input(int key)
{
    if (scroll_list->handle_input(key)){
        if(Screen::handle_input(key)){
            if(!newsgroup)
                return key;
            Uint32 max_size = newsgroup->postsets.size();
            switch(key){
                case IKEY_LEFTARROW:
                    if (postset_index){
                        --postset_index;
                    }
                    return 0; break;
                    
                case IKEY_RIGHTARROW:
                    if (postset_index < max_size){
                        ++postset_index;
                    }
                    return 0; break;
                    
                default:
                    return key;
            }
        }
    }    
    return 0;
}

void PostSetDetailScreen::handle_selection(Uint32 index)
{
    //console->net_thread->set_retrieve();
}

Uint32 PostSetDetailScreen::search_match(string search, void *ptr)
{
    return 1;
}
