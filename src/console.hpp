#ifndef ___console_inc
#define ___console_inc 1

#include<list>
#include<iostream> // Evil

#include<SDL_types.h>

#include<sstream>

#include"post_set.hpp"

#define MAKE_STRING( msg )  ( ((std::ostringstream&)(std::ostringstream() << msg)).str() )

using std::list;
using std::string;

class Console {
    public:
        // Public data members go here.
        Console(); // Constructor
        ~Console(); // Destructor

        void render(Uint32 top, Uint32 bottom);
        void check_input(char key);
        void log(std::string line);
        void fatal(std::string line);
        void box_log(std::string line);
        void draw_box(Uint32 x, Uint32 y, Uint32 width, Uint32 height);


        list<std::string> loglines;
        string input;
        Uint32 print_logs;
        Uint32 print_on_delete;
        Uint32 keep_logs;

    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

extern Console *console;

#endif
