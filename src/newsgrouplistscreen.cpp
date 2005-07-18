#include "newsgrouplistscreen.hpp"
#include"globals.hpp"
#include"keydefs.hpp"
#include<ncurses.h>
#include<sstream>
#include<vector>

using std::stringstream;
using std::vector;

NewsGroupListScreen::NewsGroupListScreen()
{
    ng_index = 0;
    scroll_index = 0;
}

NewsGroupListScreen::~NewsGroupListScreen()
{
}

void NewsGroupListScreen::render(void)
{
    Screen::render();
    string str;
    stringstream buf;
    Uint32 yindex = ypos + 2;
    vector <NewsGroup *> ngs = newsgroups;
    Uint32 max_size = ngs.size() > (height -3) 
                    ? height-3
                    : ngs.size() - scroll_index;
    
    for (Uint32 i = 0; i < max_size; ++i){
        buf << ngs[scroll_index + i]->name;
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

        switch(key){
            case KEY_ENTER:
                exit(0);
                console->log("Switching");
                flash();
                session->switch_postset_list(newsgroups[ng_index]);
                return 0;
                break;
            case KEY_UPARROW:
                if (ng_index){
                    --ng_index;
                }
                if (ng_index < scroll_index)
                    scroll_index = ng_index; 
                return 0;
                break;
            case KEY_DOWNARROW:
                if (ng_index < max_size){
                    ++ng_index;
                }
                while (ng_index > (scroll_index + render_size -2))
                    ++scroll_index; 
                return 0;
                break;
                
            default:
                return 1;
        }
    }
    return 0;
}

