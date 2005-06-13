#include "console.hpp"
#include <ncurses.h>


// Public data members go here.
Console::Console() // Constructor
{
    xres=100;
    yres=10;
    print_logs = 0;
    box_log("Console Initialized");
    input = "";
}

Console::Console(Uint16 x, Uint16 y) // Constructor
{
    xres=x;
    yres=y;
    box_log("Console Initialized");
}
    
Console::~Console() // Destructor
{
}

void Console::render(void)
{
    std::list<std::string>::iterator i;
    Sint16 counter=yres-2;

    erase();

    for(i = loglines.begin() ; (i != loglines.end()) && counter != -1; ++i)
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
            std::string line;
            line = "> " + input;
            log(line);
            input = "";
        }else {
            input += (char)key;
        }
    }
}

void Console::log(std::string line)
{
    loglines.push_front(line);
    if(print_logs){
        printf("%s\n", line.c_str());
    }
}

void Console::fatal(std::string line)
{
    log(line);
    SDL_Quit();
}

void Console::box_log(std::string line)
{
    std::string Border(line.length() + 2, '-');

    log("+" + Border + "+");
    log("| " + line + " |");
    log("+" + Border + "+");
}



// Private members go here.
// Protected members go here.
