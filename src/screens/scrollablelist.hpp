#ifndef ___scrollablelist_inc
#define ___scrollablelist_inc 1

#include"SDL_types.h"
#include"widget.hpp"
#include"screen.hpp"
#include"keydefs.hpp"

#include<string>
#include<sstream>

using std::string;
using std::stringstream;
/*
 * How This All Works: (or should)
 *     template needs to know a) type of item to be displayed (why?  the screen displays it?) 
 *                            b) 
 *     wow I'm lost :(
 */

template <class T>
class ScrollableList : public Widget {
    public:
        ScrollableList();
        ~ScrollableList();

        virtual int handle_input(int key);
        virtual void render(void);
        virtual void refine_search(void);

        Uint32 is_searching;
        string search_string;
        vector<T *> my_items;
        vector<T *> all_items;
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
        buf << " /" << search_string << " ";
        str = buf.str();
        mvaddnstr(height-1, xpos + 1, str.c_str(), -1);
        buf.str("");
        refine_search();
    }
        
    if(known_size != all_items.size()){
        known_size = all_items.size();
        refine_search();
    }

    Uint32 max_size = my_items.size() > (height -3) 
                    ? height-3
                    : my_items.size() - scroll_index;
    
    for (Uint32 i = 0; i < max_size; ++i){

        if(pos_index == (yindex - (ypos + 2) + scroll_index)) {
            color_set(2, NULL);
            mvaddnstr(yindex, xpos + 1, "**", -1);
        }

        screen->render_scrollable_line(yindex, xpos + 3, width, my_items[scroll_index + i]);

        color_set(1, NULL);
        ++yindex;
    }
}

template <class T>
void ScrollableList<T>::refine_search(void)
{
    my_items.clear();
    Uint32 max = all_items.size();
    for (Uint32 i = 0; i < max; ++i){
        if (screen->search_match(search_string, all_items[i])){
            my_items.push_back(all_items[i]);
        }
    }
    if (pos_index > my_items.size()){
        pos_index = my_items.size() -1;
    }
}

template <class T>
int ScrollableList<T>::handle_input(int key)
{
    Uint32 max_size = all_items.size();

    if (is_searching){
        switch(key){
            case IKEY_ENTER:
                is_searching = 0;
                break;
                
            case IKEY_UPARROW:
                if (pos_index){
                    --pos_index;
                }
                if (pos_index < scroll_index)
                    scroll_index = pos_index; 
                return 0; break;
                
            case IKEY_DOWNARROW:
                if (pos_index < (max_size-1)){
                    ++pos_index;
                }
                while (pos_index > (scroll_index + height -2))
                    ++scroll_index; 
                return 0; break;

            case IKEY_RIGHTARROW:
                screen->handle_selection(my_items[pos_index]);
                return 0; break;

            default:
                search_string += key;
                break;
         }
    }else{
        switch(key){
            case IKEY_ENTER:
            case IKEY_RIGHTARROW:
                screen->handle_selection(my_items[pos_index]);
                return 0; break;
                
            case IKEY_UPARROW:
                if (pos_index){
                    --pos_index;
                }
                if (pos_index < scroll_index)
                    scroll_index = pos_index; 
                return 0; break;
                
            case IKEY_DOWNARROW:
                if (pos_index < (max_size-1)){
                    ++pos_index;
                }
                while (pos_index > (scroll_index + height -2))
                    ++scroll_index; 
                return 0; break;
                
            case IKEY_SLASH:
                my_items = all_items;
                is_searching = 1;
                search_string = "";
                return 0; break;
                
            default:
                return 1;
        }
    }
    return 0;
}

#endif

