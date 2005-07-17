#include "session.hpp"
#include"globals.hpp"
#include"postsetlistscreen.hpp"
#include"postsetdetailscreen.hpp"
#include"newsgrouplistscreen.hpp"

Session::Session()
{
    current_screen_id = 0;
    widgets.resize(2);
    widgets[1] = new PostSetDetailScreen();
    widgets[0] = new PostSetListScreen();
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

void Session::switch_postset_detail(NewsGroup *newsgroup, int postset_no)
{
    PostSetDetailScreen *scr = (PostSetDetailScreen *) widgets[1];
    scr->newsgroup = newsgroup;
    scr->postset_index = postset_no;
    current_screen_id = 1;
}
