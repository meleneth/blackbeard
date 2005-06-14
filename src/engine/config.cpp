#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>  // I/O 
#include <fstream>   // file I/O

#include "config.hpp"
#include "console.hpp"

#define CONFIGFILENAME "/.blackbeardrc"

using std::ofstream;
using std::ifstream;
using std::ios;
using std::endl;
using std::string;

// Public data members go here.
Config::Config() // Constructor
{
    news_server = "localhost";
    news_group  = "misc.test";
    home_dir = getenv("HOME");
    blackbeard_dir = home_dir + "/blackbeard";
    config_filename = home_dir + CONFIGFILENAME;

}
    
Config::~Config() // Destructor
{
}

void Config::read_config_file(void)
{
    struct stat my_stats;
    char linebuffer[1024];

    console->log("reading " + config_filename);
    if(stat(config_filename.c_str(), &my_stats) == -1){
        console->log("Initializing config files");
        setup_files();
    }

    ifstream in;
    in.open(config_filename.c_str(), ios::in);
    in.getline(linebuffer, 1024);

    while(!in.eof()){
        std::string line = linebuffer;
        unsigned int index;
        index = line.find_first_of("#");

        if(index != string::npos)
            line.erase(index);

        index = line.find("=");
        if(index != string::npos){
            std::string cmd = line.substr(0, index);
            std::string value = line.substr(index+1, line.size() - (index +1));

            if(0 == cmd.compare("news_server")){
                news_server = value;
            } else if(0 == cmd.compare("news_group")){
                news_group = value;
            } else {
                console->log("Unknown line: " + line);
            }
        }
        in.getline(linebuffer, 1024);
    }
}

void Config::setup_files(void)
{
    mkdir(blackbeard_dir.c_str(), 01777);

    ofstream out;
    out.open(config_filename.c_str(), ios::out);
    out << "news_server=localhost" << endl;
    out << "news_group=misc.test" << endl;
    out.close();
}

// Private members go here.
// Protected members go here.
