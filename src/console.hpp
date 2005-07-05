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
        Console(Uint16 x, Uint16 y); // Constructor
        ~Console(); // Destructor

        void render(void);
        void check_input(char key);
        void log(std::string line);
        void fatal(std::string line);
        void box_log(std::string line);
        void draw_box(Uint32 x, Uint32 y, Uint32 width, Uint32 height);

        void render_current_postset(PostSet *set, Uint32 postset_no, Uint32 num_postsets);

        list<std::string> loglines;
        string input;
        Uint16 xres, yres;
        Uint32 print_logs;
        Uint32 print_on_delete;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

extern Console *console;

#endif
