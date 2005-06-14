#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>  // I/O 
#include <fstream>   // file I/O

#include "config.hpp"
#include "console.hpp"

#define CONFIGFILENAME "/.blackbeardrc"

using std::ofstream;
using std::ios;
using std::endl;

// Public data members go here.
Config::Config() // Constructor
{
    news_server = "localhost";
    news_group  = "misc.test";
    home_dir = strdup(getenv("HOME"));
    blackbeard_dir = home_dir + "/blackbeard";
    config_filename = home_dir + CONFIGFILENAME;

}
    
Config::~Config() // Destructor
{
}

void Config::read_config_file(void)
{
    struct stat my_stats;


    console->log("reading " + config_filename);
    if(stat(config_filename.c_str(), &my_stats) == -1){
        setup_files();
    }else{
        console->log("I want to load it but you didnt teach me how");
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
