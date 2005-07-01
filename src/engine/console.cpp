#include "console.hpp"
#include <ncurses.h>

#include"globals.hpp"

using std::string;
using std::stringstream;
using std::list;

// Public data members go here.
Console::Console() // Constructor
{
    xres=100;
    yres=10;
    print_logs = 0;
    box_log("Console Initialized");
    input = "";
    print_on_delete = 0;
}

Console::Console(Uint16 x, Uint16 y) // Constructor
{
    xres=x;
    yres=y;
    box_log("Console Initialized");
}
    
Console::~Console() // Destructor
{
    if(print_on_delete){
        list<string>::iterator i;
        for(i = loglines.end(); ;)
        {
            --i;
            printf("%s\n", i->c_str());
            if ( i == loglines.begin()){
                break;
            }
        }
    }
}

void Console::render(void)
{
    list<string>::iterator i;
    Sint16 counter=yres-3;
    Uint32 num_postsets = 0;
    Uint32 num_postfiles = 0;
    Uint32 current_postset_no = 0;

    erase();

    if(newsgroup){
        string groupline = newsgroup->name + " ( " + newsgroup->status + " )";
        mvaddnstr(0, 0, groupline.c_str(), -1);
        
        list<PostSet *>::iterator p;
        attron(A_BOLD);
        for(p = newsgroup->postsets.begin() ; p != newsgroup->postsets.end() ; ++p){
            mvaddnstr(1, 0, (*p)->status().c_str(), -1);
            num_postsets++;
            num_postfiles += (*p)->num_files;
            if(current_postset == *p){
                current_postset_no = num_postsets;
            }
        }
        if(current_postfile){
            mvaddnstr(yres-2, 0, current_postfile->status().c_str(), -1);

            stringstream buf;
            buf << "(" << current_postset_no << "/" << num_postsets << ") postsets";
            mvaddnstr(1, xres - (buf.str().length() + 2), buf.str().c_str(), -1);
            mvaddnstr(yres-2, 0, current_postfile->status().c_str(), -1);
        }
        attroff(A_BOLD);
        
    }

    for(i = loglines.begin() ; (i != loglines.end()) && counter > 4; ++i)
    {
        mvaddnstr(counter, 0,  i->c_str(), -1);
        counter--;
    }
    refresh();
}

void Console::check_input(char key)
{
    if(key != ERR){
        if(key == 13){
            string line;
            line = "> " + input;
            log(line);
            input = "";
        }else {
            input += (char)key;
        }
    }
}

void Console::log(string line)
{
    loglines.push_front(line);
    if(print_logs){
        printf("%s\n", line.c_str());
    }
}

void Console::fatal(string line)
{
    log(line);
}

void Console::box_log(string line)
{
    string Border(line.length() + 2, '-');

    log("+" + Border + "+");
    log("| " + line + " |");
    log("+" + Border + "+");
}



// Private members go here.
// Protected members go here.
