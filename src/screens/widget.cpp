#include "widget.hpp"
#include"config.hpp"
#include"console.hpp"
#include"session.hpp"
#include"keydefs.hpp"
#include"post_set.hpp"
#ifdef __WIN32__
#include<curses.h>
#else
#include<ncurses.h>
#endif

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
        case IKEY_Q:
        case IKEY_ESC:
            session->switch_shutdown_screen();
            save_subscribed_groups_to(config->blackbeard_data_dir + "/subscribed_newsgroups");
            endwin();
            delete console;
            exit(0);
            break;

        default:
            return key;
    }
}
