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
    is_searching = 0;
    search_string = "";
    known_size = 0;
}

PostSetListScreen::~PostSetListScreen()
{
}

void PostSetListScreen::refine_search(void)
{
    my_postsets.clear();
    Uint32 max = newsgroup->postsets.size();
    for (Uint32 i = 0; i < max; ++i){
        if (newsgroup->postsets[i]->subject.find(search_string) != string::npos){
            my_postsets.push_back(newsgroup->postsets[i]);
        }
    }
}

void PostSetListScreen::render(void)
{
    Screen::render();
    string str;
    stringstream buf;
    Uint32 yindex = ypos + 2;

    mvaddnstr(ypos, xpos + 1, "PostSetListScreen::render", -1);
    if(!newsgroup)
        return;

    if(known_size != newsgroup->postsets.size()){
        my_postsets = newsgroup->postsets;
        known_size = my_postsets.size();
    }

    Uint32 max_size = my_postsets.size() > (height -3) 
                    ? height-3
                    : my_postsets.size() - scroll_index;
    
    for (Uint32 i = 0; i < max_size; ++i){
        buf << my_postsets[scroll_index + i]->subject;
        str = buf.str();
        mvaddnstr(yindex, xpos + 3, str.c_str(), -1);
        if(postset_index == (yindex - (ypos +2) + scroll_index)) {
            color_set(2, NULL);
            mvaddnstr(yindex, xpos + 1, "**", -1);
            color_set(1, NULL);
        }
        buf.str("");
        ++yindex;
    }
}

int PostSetListScreen::handle_input(int key)
{
    if(Screen::handle_input(key)){
        if(!newsgroup)
            return key;

        Uint32 max_size = newsgroup->postsets.size();
        Uint32 render_size = height-2;

        if (is_searching){
            switch(key){
                case IKEY_ENTER:
                    is_searching = 0;
                    search_string = "";
                    break;
                default:
                    search_string += key;
                    refine_search();
                    break;
             }
        }else{
            switch(key){
                case IKEY_ENTER:
                    console->log("Switching");
                    flash();
                    session->switch_postset_detail(newsgroup, postset_index);
                    return 0; break;
                    
                case IKEY_UPARROW:
                    if (postset_index){
                        --postset_index;
                    }
                    if (postset_index < scroll_index)
                        scroll_index = postset_index; 
                    return 0; break;
                    
                case IKEY_DOWNARROW:
                    if (postset_index < max_size){
                        ++postset_index;
                    }
                    while (postset_index > (scroll_index + render_size -2))
                        ++scroll_index; 
                    return 0; break;
                    
                case IKEY_SLASH:
                    my_postsets = newsgroup->postsets;
                    is_searching = 1;
                    search_string = "";
                    return 0; break;
                    
                default:
                    return 1;
            }
        }
    }
    return 0;
}
