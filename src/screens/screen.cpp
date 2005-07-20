#include "screen.hpp"
#include"globals.hpp"

#include<ncurses.h>

Screen::Screen()
{
    xpos=0;
    ypos=0;
    help_visible = 0;
}

Screen::~Screen()
{
}

void Screen::render(void)
{
    width = COLS;
    height = LINES;

    console->draw_box(0, 0, width-1, height-1);

    if(help_visible){
        render_help();
    }else{
        Uint32 max_size = widgets.size();
        for(Uint32 w = 0; w < max_size; ++w){
            widgets[w]->render();
        }
    }

}

void Screen::render_help(void)
{
    mvaddnstr(ypos+2, xpos + 2, "No help is available for this screen.", -1);
}

void Screen::handle_selection(void *ptr)
{
    //pure virtual?
}

void Screen::render_scrollable_line(Uint32 y, Uint32 x, Uint32 width, void *ptr)
{
}

Uint32 Screen::search_match(string search, void *ptr)
{
}


int Screen::handle_input(int key)
{
    key = Widget::handle_input(key);
    switch(key){
        case 0:
            return 0;
        case 'h':
        case '?':
        case 'H':
            help_visible = !help_visible;
            break;
        default:
            return key;
    }
    return 0;
}
