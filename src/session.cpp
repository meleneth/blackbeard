#include "session.hpp"
#include"globals.hpp"
#include"postsetlistscreen.hpp"
#include"postsetdetailscreen.hpp"
#include"newsgrouplistscreen.hpp"

Session::Session()
{
    current_screen_id = 0;
    widgets.push_back(new PostSetDetailScreen());
    widgets.push_back(new PostSetListScreen());
//    widgets->push_back(new NewsGroupListScreen());
}

Session::~Session()
{
}

void Session::render(void)
{
    widgets[current_screen_id]->render();
}

int Session::handle_input(int key)
{
    return widgets[current_screen_id]->handle_input(key);
}

void Session::switch_screen(void)
{
    ++current_screen_id;
    current_screen_id %= widgets.size();
    return;
}
