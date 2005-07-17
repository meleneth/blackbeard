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
                    : psets.size();
    
    for (Uint32 i = scroll_index; i < max_size; ++i){
        buf << psets[i]->subject;
        str = buf.str();
        mvaddnstr(yindex, xpos + 3, str.c_str(), -1);
        if(postset_index == (yindex - (ypos +2) + scroll_index)) 
            mvaddnstr(yindex, xpos + 1, "**", -1);

        ++yindex;
        buf.str("");
    }
}

int PostSetListScreen::handle_input(int key)
{
    if(Screen::handle_input(key)){
        Uint32 max_size = newsgroup->postsets.size();
        console->log("Got switch cmd");

        switch(key){
            case KEY_ENTER:
                return 0;
                break;
            case KEY_UPARROW:
                if (postset_index){
                    --postset_index;
                }
                if(max_size){
                    //console->current_postset = newsgroup->postsets[postset_index];
                }
                return 0;
                break;
            case KEY_DOWNARROW:
                if (postset_index < max_size){
                    ++postset_index;
                }
                if(max_size){
                    //console->current_postset = newsgroup->postsets[postset_index];
                }
                return 0;
                break;
                
            default:
                return 1;
        }
    }
    return 0;
}
