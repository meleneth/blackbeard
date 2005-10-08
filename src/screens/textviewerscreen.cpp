#include"textviewerscreen.hpp"
#include"jobqueue.hpp"
#include"netcentral.hpp"
#include"console.hpp"
#include"postfilejob.hpp"

TextViewerScreen::TextViewerScreen()
{
    text_list = new ScrollableList<NewsGroupPost>;
    text_list->screen = this;
    widgets.push_back(text_list);
    file = NULL;
}

TextViewerScreen::~TextViewerScreen()
{
}

void TextViewerScreen::handle_selection(Uint32 index)
{
}

void TextViewerScreen::render_scrollable_line(Uint32 yindex, Uint32 x, Uint32 width, void *newsgroupost)
{
    NewsGroupPost *current_post = (NewsGroupPost *) newsgroupost;
    if(current_post){
        // mvaddnstr(yindex, x, current_post->lines().c_str(), -1);
    }

    if (file) {
        console->log("rendering line");
        string is_loading = "Loading file: " + file->filename;
        console->log(is_loading);
        mvaddnstr(yindex++, xpos + 2, (char*)is_loading.c_str(), -1);
    }
}

void TextViewerScreen::render(void)
{
    text_list->height = (height - 3);
    text_list->width = width -1;
    text_list->ypos = ypos + 1;
//    text_list->all_items = jobqueue->active_jobs;

    Screen::render();
    console->draw_box(0, 0, width-1, height-1);

    mvaddnstr(ypos, xpos + 1, "TextViewerScreen::render", -1);
}

void TextViewerScreen::render_help(void)
{
    Uint32 yindex = ypos +1;
    
    mvaddnstr(yindex++, xpos + 2, "This screen shows you the contents of any valid text file", -1);
}

Uint32 TextViewerScreen::search_match(string search, void *ptr)
{
    return 0;
//    return ((NewsGroupPost *)ptr)->status_line().find(search) != string::npos;
}

int TextViewerScreen::handle_input(int key)
{
    return Screen::handle_input(key);
}

void TextViewerScreen::set_file(PostFile *post_file)
{
    console->log("Got file: " + post_file->filename);
    file = post_file;
    PostfileJob *job = new PostfileJob(file);
    console->log("processing job");
    job->process();
}
