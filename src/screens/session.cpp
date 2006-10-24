#include "session.hpp"
#include "postsetlistscreen.hpp"
#include "postsetdetailscreen.hpp"
#include "newsgrouplistscreen.hpp"
#include "activejobsscreen.hpp"
#include "activenetjobsscreen.hpp"
#include "textviewerscreen.hpp"

Session::Session()
{
    newsgrouplist       = new NewsGroupListScreen();
    postsetlist         = new PostSetListScreen();
    postsetdetail       = new PostSetDetailScreen();
    activejobs          = new ActiveJobsScreen();
    activenetjobs       = new ActiveNetJobsScreen();
    textviewerlist      = new TextViewerScreen();
    downloaded_postsets = new DownloadedPostSetsScreen();

    current_screen = newsgrouplist;
}

Session::~Session()
{
    current_screen = new ShutDownScreen();
    delete textviewerlist;
    delete newsgrouplist;
    delete postsetlist;
    delete postsetdetail;
    delete downloaded_postsets;
    delete current_screen;
    current_screen = NULL;
}

void Session::render(void)
{
    if(current_screen)
        current_screen->render();
}

int Session::handle_input(int key)
{
    return current_screen->handle_input(key);
}

void Session::switch_postset_detail(NewsGroup *newsgroup, Uint32 selected_postset_no)
{
    postsetdetail->newsgroup = newsgroup;
    if(postsetdetail->postset_index != selected_postset_no) {
        postsetdetail->scroll_list->pos_index = 0;
    }
    postsetdetail->postset_index = selected_postset_no;
    current_screen = postsetdetail;
}
        
void Session::switch_postset_list(NewsGroup *newsgroup)
{
    postsetlist->newsgroup = newsgroup;
    current_screen = postsetlist;
}

void Session::switch_shutdown_screen(void)
{
    current_screen = new ShutDownScreen();
}
