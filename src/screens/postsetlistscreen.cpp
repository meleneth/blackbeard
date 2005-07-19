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
    string str1;
    stringstream buf;
    Uint32 yindex = ypos + 2;

    mvaddnstr(ypos, xpos + 1, "PostSetListScreen::render", -1);

    if (help_visible)
        return;

    if(!newsgroup)
        return;

    my_postsets = newsgroup->postsets;
    if(is_searching){
        buf << "/" << search_string;
        str = buf.str();
        mvaddnstr(height-1, xpos + 1, str.c_str(), -1);
        buf.str("");
        refine_search();
    }
        
    if(known_size != newsgroup->postsets.size()){
        my_postsets = newsgroup->postsets;
        known_size = my_postsets.size();
    }

    Uint32 max_size = my_postsets.size() > (height -3) 
                    ? height-3
                    : my_postsets.size() - scroll_index;
    
    for (Uint32 i = 0; i < max_size; ++i){
        PostSet *set = my_postsets[scroll_index + i];
        set->recalculate_piece_info();
        
        buf << set->subject;
        str = buf.str();
        mvaddnstr(yindex, xpos + 3, str.c_str(), -1);
        buf.str("");
        
        buf << "[" << set->num_files << "/" << set->max_num_files << "]";
        str1 = buf.str();
        mvaddnstr(yindex, width - str1.length() - 3, str1.c_str(), -1);
        buf.str("");
        
        if(postset_index == (yindex - (ypos +2) + scroll_index)) {
            color_set(2, NULL);
            mvaddnstr(yindex, xpos + 1, "**", -1);
            mvaddnstr(yindex, xpos + 3, str.c_str(), -1);
            mvaddnstr(yindex, width - str1.length() - 3, str1.c_str(), -1);
            color_set(1, NULL);
        }
        ++yindex;
    }
}

void PostSetListScreen::render_help(void)
{
    Uint32 yindex = ypos +1;
    
    mvaddnstr(yindex++, xpos + 2, "This screen shows you a list of post sets. hit enter to see a list of files.", -1);

}

int PostSetListScreen::handle_input(int key)
{
    if(!newsgroup)
        return key;

    Uint32 max_size = newsgroup->postsets.size();
    Uint32 render_size = height-2;

    if (is_searching){
        switch(key){
            case IKEY_ENTER:
                is_searching = 0;
                break;
                
            case IKEY_UPARROW:
                if (postset_index){
                    --postset_index;
                }
                if (postset_index < scroll_index)
                    scroll_index = postset_index; 
                return 0; break;
                
            case IKEY_DOWNARROW:
                if (postset_index < (max_size-1)){
                    ++postset_index;
                }
                while (postset_index > (scroll_index + render_size -2))
                    ++scroll_index; 
                return 0; break;

            case IKEY_RIGHTARROW:
                session->switch_postset_detail(newsgroup, postset_index);
                return 0; break;

            default:
                search_string += key;
                break;
         }
    }else{
        if(Screen::handle_input(key)){
            switch(key){
                case IKEY_ENTER:
                case IKEY_RIGHTARROW:
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
                    if (postset_index < (max_size-1)){
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
