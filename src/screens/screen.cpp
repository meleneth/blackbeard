#include"screen.hpp"
#include"session.hpp"
#include"console.hpp"
#include"keydefs.hpp"
#include"jobqueue.hpp"
#include"netcentral.hpp"

#include<ncurses.h>
#include<sstream>

using std::stringstream;

Screen::Screen()
{
    xpos=0;
    ypos=0;
    help_visible = 0;
    active_widget = 31337;
}

Screen::~Screen()
{
}

void Screen::render(void)
{
    width = COLS;
    height = LINES;

    height--;
    console->draw_box(0, 0, width-1, height-1);

    if(help_visible){
        render_help();
    }else{
        Uint32 max_size = widgets.size();
        for(Uint32 w = 0; w < max_size; ++w){
            widgets[w]->render();
        }
    }

    stringstream buf;
    buf << netcentral->jobs.size() << " jobs queued " << netcentral->krate() << "k/sec 0G downloaded";
    mvaddnstr(ypos + height, xpos + 2, buf.str().c_str(), -1);

}

void Screen::render_help(void)
{
    mvaddnstr(ypos+2, xpos + 2, "No help is available for this screen.", -1);
}

void Screen::handle_selection(Uint32 index)
{
    //pure virtual?
}

void Screen::render_scrollable_line(Uint32 y, Uint32 x, Uint32 width, void *ptr)
{
}

Uint32 Screen::search_match(string search, void *ptr)
{
    return 0;
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

        case IKEY_TAB:
            if(31337 == active_widget){
                if(widgets.size()){
                    active_widget = 0;
                }
                break;
            }
            ++active_widget;
            active_widget %= widgets.size();
            break;
            
        case IKEY_F5:
            session->current_screen = session->newsgrouplist;
            break;

        case IKEY_F6:
            session->current_screen = session->postsetlist;
            break;

        case IKEY_F7:
            session->current_screen = session->postsetdetail;
            break;

        case IKEY_F8:
            session->current_screen = session->activejobs;
            break;

        case IKEY_F9:
            session->current_screen = session->activenetjobs;
            break;

        default:
            return key;
    }
    return 0;
}
