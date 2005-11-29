#include"postsetdetailscreen.hpp"
#include"keydefs.hpp"
#include"console.hpp"
#include"netcentral.hpp"
#include"postfilejob.hpp"
#include"session.hpp"

#include<sstream>
#ifdef __WIN32__
#include<curses.h>
#else
#include<ncurses.h>
#endif

using std::stringstream;

PostSetDetailScreen::PostSetDetailScreen()
{
    scroll_list = new ScrollableList<PostFile>;
    scroll_list->screen = this;
    widgets.push_back(scroll_list);
    newsgroup = NULL;
}

PostSetDetailScreen::~PostSetDetailScreen()
{
}

void PostSetDetailScreen::render_help(void)
{
    Uint32 yindex = ypos +1;
    
    mvaddnstr(yindex++, xpos + 2, "This screen shows you a list of post files for the current post set.", -1);
}

void PostSetDetailScreen::render(void)
{
    Uint32 num_postsets = 0;

    if(newsgroup) {
        num_postsets = newsgroup->postsets.size();
        if(postset_index > num_postsets){
            postset_index = 0;
        }
    }
    
    Screen::render();
    if(help_visible)
        return;
    
    Uint32 three_quarters = (height * 3) / 4;

    scroll_list->width = width -1;
    scroll_list->ypos = ypos + 4;
    scroll_list->height = three_quarters - scroll_list->ypos;

    console->draw_box(0, 0, width-1, three_quarters);
    mvaddnstr(ypos, xpos + 1, "PostSetDetailScreen::render", -1);

    if(newsgroup){
        string groupline = newsgroup->name + " ( " + newsgroup->status + " )";
        mvaddnstr(1, 1, (char*)groupline.c_str(), -1);
        
        attron(A_BOLD);
        PostSet *set = NULL;
        if(num_postsets){
            set = newsgroup->postsets[postset_index];
        }

        if(set){
            scroll_list->all_items = set->files;
            mvaddnstr(2, 1, (char*)set->status().c_str(), -1);

            stringstream buf;
            buf << "(" << postset_index << "/" << num_postsets << ") postsets";
            string str = buf.str();
            mvaddnstr(3, COLS - (str.length() + 3), (char*)str.c_str(), -1);
        }
        attroff(A_BOLD);
    }

    console->render(three_quarters, height-4);
}

void PostSetDetailScreen::render_scrollable_line(Uint32 y, Uint32 x, Uint32 width, void *ptr)
{
    if(ptr){
        string completed_bar;
        completed_bar = ((PostFile *) ptr)->get_bar();
        mvaddnstr(y, 1, (char*)((PostFile *) ptr)->status_string().c_str(), -1);
        mvaddnstr(y, COLS - completed_bar.length() -3, (char*)completed_bar.c_str(), -1);
    }
}

void PostSetDetailScreen::render_current_postset(PostSet *set, Uint32 postset_no, Uint32 num_postsets, Uint32 lowest_line)
{
}

int PostSetDetailScreen::handle_input(int key)
{
    PostFile *f = NULL;
    if (scroll_list->handle_input(key)){
        if(Screen::handle_input(key)){
            if(!newsgroup)
                return key;
            Uint32 max_size = newsgroup->postsets.size();
            switch(key){
                case IKEY_LEFTARROW:
                    if (postset_index){
                        --postset_index;
                        scroll_list->pos_index = 0;
                    }
                    return 0; break;
                    
                case IKEY_RIGHTARROW:
                    if (postset_index < max_size){
                        ++postset_index;
                        scroll_list->pos_index = 0;
                    }
                    return 0; break;
                case 'd':
                    f = (PostFile *) scroll_list->get_selected_item();
                    netcentral->add_job(new PostfileJob(f));
                    return 0; break;

                case 'v':
                    console->log("checking for the current file");
                    f = (PostFile *) scroll_list->get_selected_item();
                    if (strstr((char*)f->filename.c_str(), ".nfo")) {
                        session->textviewerlist->set_file(f);
                    }

                    session->current_screen = session->textviewerlist;
                    return 0; break;

                default:
                    return key;
            }
        }
    }    
    return 0;
}

void PostSetDetailScreen::handle_selection(Uint32 index)
{
    //console->net_thread->set_retrieve();
}

Uint32 PostSetDetailScreen::search_match(string search, void *ptr)
{
    return 1;
}
