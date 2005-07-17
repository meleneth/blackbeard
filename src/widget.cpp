#include "widget.hpp"
#include"globals.hpp"
#include"keydefs.hpp"
#include<ncurses.h>

Widget::Widget()
{
    xpos = 0;
    ypos = 0;
    width = 0;
    height = 0;
}

Widget::~Widget()
{
}

void Widget::render()
{
}


int Widget::handle_input(int key)
{
    switch(key){
        case KEY_TAB:
            session->switch_screen();
            return 0;
        case KEY_Q:
        case KEY_ESC:
            endwin();
            delete console;
            exit(0);
            break;

        default:
            return key;
    }
}
