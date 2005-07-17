#include "postsetlistscreen.hpp"
#include"globals.hpp"
#include"keydefs.hpp"
#include<ncurses.h>
#include<sstream>

using std::stringstream;

PostSetListScreen::PostSetListScreen()
{
    postset_index = 0;
    scroll_index = 0;
}

PostSetListScreen::~PostSetListScreen()
{
}

void PostSetListScreen::render(void)
{
    Screen::render();
    string str;
    stringstream buf;
    Uint32 yindex = ypos + 2;
    vector <PostSet *> psets = newsgroup->postsets;
    Uint32 max_size = psets.size() > (height -3) 
                    ? height-3
                    : psets.size() - scroll_index;
    
    for (Uint32 i = 0; i < max_size; ++i){
        buf << psets[scroll_index + i]->subject;
        str = buf.str();
        mvaddnstr(yindex, xpos + 3, str.c_str(), -1);
        if(postset_index == (yindex - (ypos +2) + scroll_index)) 
            color_set(2, NULL);
            mvaddnstr(yindex, xpos + 1, "**", -1);
            color_set(1, NULL);
        buf.str("");
        ++yindex;
    }
}

int PostSetListScreen::handle_input(int key)
{
    if(Screen::handle_input(key)){
        Uint32 max_size = newsgroup->postsets.size();
        Uint32 render_size = height-2;

        switch(key){
            case KEY_ENTER:
                return 0;
                break;
            case KEY_UPARROW:
                if (postset_index){
                    --postset_index;
                }
                if (postset_index < scroll_index)
                    scroll_index = postset_index; 
                return 0;
                break;
            case KEY_DOWNARROW:
                if (postset_index < max_size){
                    ++postset_index;
                }
                while (postset_index > (scroll_index + render_size -2))
                    ++scroll_index; 
                return 0;
                break;
                
            default:
                return 1;
        }
    }
    return 0;
}
