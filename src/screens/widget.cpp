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
        case IKEY_TAB:
            session->switch_screen();
            return 0;
        case IKEY_Q:
        case IKEY_ESC:
            save_subscribed_groups_to(config->blackbeard_data_dir + "/subscribed_newsgroups");
            endwin();
            delete console;
            exit(0);
            break;

        default:
            return key;
    }
}
