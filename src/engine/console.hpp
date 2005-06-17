#ifndef ___console_inc
#define ___console_inc 1

#include<list>
#include<iostream> // Evil

#include<SDL.h>

#include<sstream>

#define MAKE_STRING( msg )  ( ((std::ostringstream&)(std::ostringstream() << msg)).str() )

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

        std::list<std::string> loglines;
        std::string input;
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
