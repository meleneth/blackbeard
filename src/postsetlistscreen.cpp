#include "postsetlistscreen.hpp"
#include"globals.hpp"
#include"keydefs.hpp"
#include<ncurses.h>
#include<sstream>

using std::stringstream;

PostSetListScreen::PostSetListScreen()
{
    postset_index = 0;
}

PostSetListScreen::~PostSetListScreen()
{
}

void PostSetListScreen::render(void)
{
    string str;
    stringstream buf;
    Uint32 yindex = 2;
    Uint32 max_size = newsgroup->postsets.size();
    for (Uint32 i = postset_index; i < max_size; ++i){
        buf << newsgroup->postsets[i]->subject;
        str = buf.str();
        mvaddnstr(yindex++, 1, str.c_str(), -1);
        buf.str("");
    }
}

int PostSetListScreen::handle_input(int key)
{
    if(Screen::handle_input(key)){
        Uint32 max_size = newsgroup->postsets.size();

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
