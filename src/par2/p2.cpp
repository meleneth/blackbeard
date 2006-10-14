#include "console.hpp"
#include "pararchive.hpp"

Console *console;
string last_msg = "let the force be with you";

int main(int argc, char *argv[])
{
    console = new Console();
    console->print_logs = 1;
    console->log("Par2 development binary");
    load_par_file(argv[1]);
}
