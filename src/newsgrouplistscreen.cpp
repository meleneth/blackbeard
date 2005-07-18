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
    ng_index = 0;
    scroll_index = 0;
    is_searching = 0;
}

NewsGroupListScreen::~NewsGroupListScreen()
{
}

void NewsGroupListScreen::refine_search(void)
{
    my_groups.clear();
    Uint32 max = newsgroups.size();
    for (Uint32 i = 0; i < max; ++i){
        if (newsgroups[i]->name.find(search_string) != string::npos){
            my_groups.push_back(newsgroups[i]);
        }
    }
}

void NewsGroupListScreen::render(void)
{
    Screen::render();
    string str;
    stringstream buf;
    Uint32 yindex = ypos + 2;

    mvaddnstr(ypos, xpos + 1, "NewsGroupListScreen::render", -1);
    
    Uint32 max_size = my_groups.size() > (height -3) 
                    ? height-3
                    : my_groups.size() - scroll_index;
    
    for (Uint32 i = 0; i < max_size; ++i){
        buf << my_groups[scroll_index + i]->name;
        str = buf.str();
        mvaddnstr(yindex, xpos + 3, str.c_str(), -1);
        if(ng_index == (yindex - (ypos +2) + scroll_index)) {
            color_set(2, NULL);
            mvaddnstr(yindex, xpos + 1, "**", -1);
            color_set(1, NULL);
        }
        buf.str("");
        ++yindex;
    }
}

int NewsGroupListScreen::handle_input(int key)
{
    if(Screen::handle_input(key)){
        Uint32 max_size = newsgroups.size();
        Uint32 render_size = height-2;
        if (is_searching){
            switch(key){
                case IKEY_ENTER:
                    is_searching = 0;
                    break;
                default:
                    search_string += key;
                    refine_search();
                    break;
             }
        }else{
            my_groups = newsgroups;
            switch(key){
                case IKEY_ENTER:
                    console->log("Switching");
                    flash();
                    session->switch_postset_list(newsgroups[ng_index]);
                    return 0; break;
                    
                case IKEY_UPARROW:
                    if (ng_index){
                        --ng_index;
                    }
                    if (ng_index < scroll_index)
                        scroll_index = ng_index; 
                    return 0; break;
                    
                case IKEY_DOWNARROW:
                    if (ng_index < max_size){
                        ++ng_index;
                    }
                    while (ng_index > (scroll_index + render_size -2))
                        ++scroll_index; 
                    return 0; break;
                    
                case IKEY_SLASH:
                    is_searching = 1;
                    return 0; break;
                    
                default:
                    return 1;
            }
        }
    }
    return 0;
}

