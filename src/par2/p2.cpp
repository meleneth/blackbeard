#include "console.hpp"
#include "pararchive.hpp"

Console *console;
string last_msg = "let the force be with you";

int main(int argc, char *argv[])
{
    console = new Console();
    console->print_logs = 1;
    console->log("Par2 development binary");
    console->log("foo.par2: " + base_par_filename("foo.par2"));
    console->log("foo.vol9.par2: " + base_par_filename("foo.vol9.par2"));
    console->log("foo.vol99.par2: " + base_par_filename("foo.vol9.par2"));
    console->log("lodg2-dcn.vol035+31.PAR2 -- " + base_par_filename("lodg2-dcn.vol035+31.PAR2"));
    console->log("TrueLiesl.cd1.vol038+34.PAR2 -- " + base_par_filename("TrueLiesl.cd1.vol038+34.PAR2"));
    console->log("lodg2-dcn.vol102+36.PAR2 -- " + base_par_filename("lodg2-dcn.vol102+36.PAR2"));
    console->log("imts-tbh.vol177+36.PAR2 -- " + base_par_filename("imts-tbh.vol177+36.PAR2"));
    console->log("imts-tbh.par2 -- " + base_par_filename("imts-tbh.par2"));
    load_par_file(argv[1]);
}
