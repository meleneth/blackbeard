#ifndef ___scrollablelist_inc
#define ___scrollablelist_inc 1

#include"SDL_types.h"
#include"widget.hpp"
#include"screen.hpp"
#include"keydefs.hpp"
#include "newsgroup.hpp"
#include "console.hpp"

#include<string>
#include<sstream>
#include<algorithm>

#ifdef __WIN32__
#include <curses.h>
#else
#include<ncurses.h>
#endif

using std::string;
using std::stringstream;

template <class T>
class ScrollableList : public Widget {
    public:
        ScrollableList();
        ~ScrollableList();

        virtual int handle_input(int key);
        virtual void render(void);
        virtual void refine_search(void);
        virtual void *get_selected_item(void);
        virtual void sort(void);

        Uint32 is_searching;
        string search_string;
        vector<T *> my_items;
        vector<T *> all_items;
        vector<Uint32> search_map;
        Uint32 pos_index;
        Uint32 scroll_index;
        Uint32 known_size;
        Screen *screen;

    private:
    protected:
};

template <class T>
ScrollableList<T>::ScrollableList()
{
    pos_index = 0;
    scroll_index = 0;
    is_searching = 0;
    search_string = "";
    known_size = 0;
    this->screen = screen;
}

template <class T>
ScrollableList<T>::~ScrollableList()
{
}

template <class T>
void ScrollableList<T>::render(void)
{
    stringstream buf;
    string str;
    string str1;
    Uint32 yindex = ypos;
    
    if(is_searching){
        buf << "|--> " << search_string << " ";
        str = buf.str();
        mvaddnstr(height+1, xpos + 1, (char*)str.c_str(), -1);
        buf.str("");
        refine_search();
    }
    
    if(pos_index > my_items.size()){
        pos_index = 0;
    }

    if(known_size != all_items.size()){
        known_size = all_items.size();
        refine_search();
    }

    if(my_items.size() ==0){
        return;
    }

    Uint32 max_size = my_items.size() > height
                    ? height
                    : my_items.size() - scroll_index;
    
    for (Uint32 i = 0; i < max_size; ++i){

        if(pos_index == (yindex - ypos + scroll_index)) {
#ifndef __WIN32__
            color_set(2, NULL);
#endif
            mvaddnstr(yindex, xpos + 1, "**", -1);
        }

        screen->render_scrollable_line(yindex, xpos + 3, width, my_items[scroll_index + i]);

#ifndef __WIN32__
        color_set(1, NULL);
#endif
        ++yindex;
    }
}

template <class T>
void ScrollableList<T>::refine_search(void)
{
    my_items.clear();
    search_map.clear();
    Uint32 max = all_items.size();
    for (Uint32 i = 0; i < max; ++i){

        if (screen->search_match(search_string, all_items[i])){
            my_items.push_back(all_items[i]);
            search_map.push_back(i);
        }
    }
    if (pos_index > my_items.size()){
        pos_index = my_items.size() -1;
    }
}

template <class T>
int ScrollableList<T>::handle_input(int key)
{
    Uint32 max_size = all_items.size() -1;

    switch(key){
        case IKEY_UPARROW:
            if (pos_index){
                --pos_index;
            }
            if (pos_index < scroll_index)
                scroll_index = pos_index; 
            return 0; break;
            
        case IKEY_DOWNARROW:
            if (pos_index < max_size){
                ++pos_index;
            }
            while (pos_index > (scroll_index + height  -1) )
                ++scroll_index; 
            return 0; break;
            
        case IKEY_SLASH:
            my_items = all_items;
            is_searching = 1;
            search_string = "";
            pos_index = 0;
            return 0; break;
            
        case IKEY_HOME:
            pos_index = 0;
            scroll_index = 0; 
            return 0; break;
            
        case IKEY_END:
            pos_index = max_size;
            while (pos_index > (scroll_index + height -1))
                ++scroll_index; 
            return 0; break;
            
        case IKEY_PGUP:
            pos_index = (pos_index > height) 
                         ? pos_index - height
                         : 0;
            scroll_index = pos_index; 
            return 0; break;
             
        case IKEY_PGDN:
            pos_index = (pos_index + height > max_size) 
                         ? max_size
                         : pos_index + height;
            while (pos_index > (scroll_index + height -1))
                ++scroll_index; 
            return 0; break;
    }
    
    if (is_searching){
        switch(key){
            case IKEY_ENTER:
                is_searching = 0;
                return 0; break;
                
            default:
                search_string += key;
                pos_index = 0;
                return 0; break;
                
         }
    } else {
        switch(key){
            case IKEY_ENTER:
                if (my_items.size()){
                    screen->handle_selection(search_map[pos_index]);
                }
                return 0; break;
        }
    }
    return key;
}

template <class T>
void *ScrollableList<T>::get_selected_item(void)
{
    if(known_size == 0)
        return NULL;
    return my_items[pos_index];

}

template <class T>
void ScrollableList<T>::sort(void)
{
    std::sort(my_items.begin(), my_items.end(), T::compare);
}

#endif

