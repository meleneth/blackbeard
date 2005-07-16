#include "postsetdetailscreen.hpp"
#include"globals.hpp"
#include"keydefs.hpp"

PostSetDetailScreen::PostSetDetailScreen()
{
}

PostSetDetailScreen::~PostSetDetailScreen()
{
}

void PostSetDetailScreen::render(void)
{
    Screen::render();
    console->render();
}

int PostSetDetailScreen::handle_input(int key)
{
    if(Screen::handle_input(key)){
        Uint32 max_size = newsgroup->postsets.size();
        switch(key){
            case 13:
                //console->net_thread->set_retrieve();
                return 0;
                break;
            case KEY_LEFTARROW:
                if (postset_index){
                    --postset_index;
                }
                if(max_size){
                    console->current_postset = newsgroup->postsets[postset_index];
                }
                return 0;
                break;
            case KEY_RIGHTARROW:
                if (postset_index < max_size){
                    ++postset_index;
                }
                if(max_size){
                    console->current_postset = newsgroup->postsets[postset_index];
                }
                return 0;
                break;
            default:
                return 1;
        }
    }
    return 0;
}
