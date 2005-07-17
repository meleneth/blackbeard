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

    console->draw_box(0, 0, width-1, height-1);

    Uint32 max_size = widgets.size();
    for(Uint32 w = 0; w < max_size; ++w){
        widgets[w]->render();
    }

}

