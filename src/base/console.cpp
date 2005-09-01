#include "console.hpp"
#include <ncurses.h>
#include<iomanip>
#include"config.hpp"

using std::string;
using std::stringstream;
using std::list;
using std::setprecision;

#define CONSOLE_MAX_SIZE 1000

// Public data members go here.
Console::Console() // Constructor
{
    print_logs = 0;
    box_log("Console Initialized");
    input = "";
    print_on_delete = 0;
    keep_logs = 1;
    save_to_file = 0;
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
    if(save_to_file){
        out.close();
    }
}

void Console::open_log_file(void)
{
    out.open("console.log", ios::out);
    save_to_file = 1;
}

void Console::render(Uint32 top, Uint32 bottom)
{
    list<string>::iterator i;
    Uint32 counter=bottom;

    for(i = loglines.begin() ; (i != loglines.end()) && counter > top; ++i)
    {
        mvaddnstr(counter, 2,  i->c_str(), -1);
        counter--;
    }
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
    extern string last_msg;
    last_msg = line;
    if(keep_logs)
        loglines.push_front(line);
    if(print_logs)
        printf("%s\n", line.c_str());
    if(save_to_file){
        out << line << endl;
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

void Console::draw_box(Uint32 x, Uint32 y, Uint32 width, Uint32 height)
{
    string w(width+1, '-');
    w[0] = '+';
    w[width] = '+';

    mvaddnstr(y, x, w.c_str(), -1);
    mvaddnstr(y+height, x, w.c_str(), -1);

    for(Uint32 yo=1; yo < height; ++yo){
        mvaddnstr(y+yo, x, "|", -1);
        mvaddnstr(y+yo, x+width, "|", -1);
    }
}

// Private members go here.
// Protected members go here.
