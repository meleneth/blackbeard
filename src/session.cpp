#include "session.hpp"
#include"globals.hpp"
#include"postsetlistscreen.hpp"
#include"postsetdetailscreen.hpp"
#include"newsgrouplistscreen.hpp"

typedef enum { ST_NEWSGROUPLIST, ST_POSTSETLIST, ST_POSTSETDETAIL } screen_types;

Session::Session()
{
    current_screen_id = 0;
    widgets.resize(3);
    widgets[ST_NEWSGROUPLIST] = new NewsGroupListScreen();
    widgets[ST_POSTSETLIST] = new PostSetListScreen();
    widgets[ST_POSTSETDETAIL] = new PostSetDetailScreen();
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
    PostSetDetailScreen *scr = (PostSetDetailScreen *) widgets[2];
    scr->newsgroup = newsgroup;
    scr->postset_index = postset_no;
    current_screen_id = ST_POSTSETDETAIL;
}
        
void Session::switch_postset_list(NewsGroup *newsgroup)
{
    PostSetListScreen *scr = (PostSetListScreen *) widgets[1];
    scr->newsgroup = newsgroup;
    current_screen_id = ST_POSTSETLIST;

}
