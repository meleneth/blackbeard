#include "session.hpp"
#include"globals.hpp"
#include"postsetlistscreen.hpp"
#include"postsetdetailscreen.hpp"
#include"newsgrouplistscreen.hpp"

Session::Session()
{
    widgets.push_front(new PostSetDetailScreen());
//    widgets->push_front(new NewsGroupListScreen());
    current_screen = new PostSetListScreen();
    widgets.push_front(current_screen);
}

Session::~Session()
{
}

void Session::render(void)
{
    if (current_screen)
        current_screen->render();
}

int Session::handle_input(int key)
{
    if(current_screen)
        current_screen->handle_input(key);
}
