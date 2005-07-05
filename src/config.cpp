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
    news_group  = "alt.text";
    username="anonymous";
    password="guest";
    home_dir = getenv("HOME");
    blackbeard_dir = home_dir + "/blackbeard";
    config_filename = home_dir + CONFIGFILENAME;
    news_port = 119;

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
        size_t index;
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
            } else if(0 == cmd.compare("news_port")){
                news_port = atoi(value.c_str());
            } else if(0 == cmd.compare("username")){
                username = value;
            } else if(0 == cmd.compare("password")){
                password = value;
            } else {
                console->log("Unknown line: " + line);
            }
        }
        in.getline(linebuffer, 1024);
    }
    in.close();
}

void Config::setup_files(void)
{
    mkdir(blackbeard_dir.c_str(), 01777);

    ofstream out;
    out.open(config_filename.c_str(), ios::out);
    out << "#news_server=localhost" << endl;
    out << "#news_group=alt.text" << endl;
    out << "#news_port=119" << endl;
    out << "#username=anonymous" << endl;
    out << "#password=guest" << endl;
    out.close();
}

// Private members go here.
// Protected members go here.