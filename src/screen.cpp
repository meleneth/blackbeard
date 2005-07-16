#include "screen.hpp"
#include"globals.hpp"

#include<ncurses.h>

Screen::Screen()
{
    xpos=0;
    ypos=0;
}

Screen::~Screen()
{
}

void Screen::render(void)
{
    width = COLS;
    height = LINES;

    list<Widget *>::iterator w;

    console->draw_box(0, 0, width-1, height-1);

    for(w = widgets.begin(); w!=widgets.end(); ++w){
        (*w)->render();
    }

}

