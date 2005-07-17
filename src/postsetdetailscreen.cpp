#include"postsetdetailscreen.hpp"
#include"globals.hpp"
#include"keydefs.hpp"

#include<sstream>
#include"ncurses.h"

using std::stringstream;

PostSetDetailScreen::PostSetDetailScreen()
{
    newsgroup = NULL;
}

PostSetDetailScreen::~PostSetDetailScreen()
{
}

void PostSetDetailScreen::render(void)
{
    Screen::render();

    Uint32 num_postsets = 0;
    Uint32 num_postfiles = 0;
    Uint32 three_quarters = (height * 3) / 4;
    Uint32 current_postset_no = 0;

    console->draw_box(0, 0, width-1, three_quarters);

    if(newsgroup){
        string groupline = newsgroup->name + " ( " + newsgroup->status + " )";
        mvaddnstr(1, 1, groupline.c_str(), -1);
        
        attron(A_BOLD);
        int max_p = newsgroup->postsets.size();
        for(int p = 0; p < max_p; ++p){
            num_postsets++;
            num_postfiles += newsgroup->postsets[p]->num_files;
            if(current_postset == newsgroup->postsets[p]){
                current_postset_no = num_postsets;
            }
        }

        render_current_postset(current_postset, current_postset_no, num_postsets, three_quarters);

        attroff(A_BOLD);
        
    }

    console->render(three_quarters, height-4);
}

void PostSetDetailScreen::render_current_postset(PostSet *set, Uint32 postset_no, Uint32 num_postsets, Uint32 lowest_line)
{
    if(!set)
        return;

    Uint32 yindex = 2;
    vector<PostFile *>::iterator i;
    vector<PostFile *> files;
    files = set->files;
    mvaddnstr(yindex, 1, set->status().c_str(), -1);

    stringstream buf;
    buf << "(" << postset_no << "/" << num_postsets << ") postsets";
    string str = buf.str();
    mvaddnstr(yindex++, COLS - (str.length() + 3), str.c_str(), -1);

    for(i = files.begin(); (i!= files.end()) && yindex < lowest_line ; ++i){
        string completed_bar;
        if(*i){
            completed_bar = (*i)->get_bar();
            mvaddnstr(yindex, 1, (*i)->status_string().c_str(), -1);
            mvaddnstr(yindex++, COLS - completed_bar.length() -3, completed_bar.c_str(), -1);
        }
    }
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
                    current_postset = newsgroup->postsets[postset_index];
                }
                return 0;
                break;
            case KEY_RIGHTARROW:
                if (postset_index < max_size){
                    ++postset_index;
                }
                if(max_size){
                    current_postset = newsgroup->postsets[postset_index];
                }
                return 0;
                break;
            default:
                return 1;
        }
    }
    return 0;
}
