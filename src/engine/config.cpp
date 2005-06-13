#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "config.hpp"
#include "console.hpp"

#define CONFIGFILENAME "/.blackbeardrc"

// Public data members go here.
Config::Config() // Constructor
{
    news_server = "localhost";
    newsgroup   = "misc.test";
}
    
Config::~Config() // Destructor
{
}

void Config::read_config_file(void)
{
    struct stat my_stats;

    std::string home_dir = strdup(getenv("HOME"));
    std::string configfilename = home_dir + CONFIGFILENAME;

    console->log("reading " + configfilename);
    if(stat(configfilename.c_str(), &my_stats) == -1){
        console->log("Not found!");
    }else{
        console->log("I want to load it but you didnt teach me how");
    }
}

void Config::setup_files(void)
{
}

// Private members go here.
// Protected members go here.
