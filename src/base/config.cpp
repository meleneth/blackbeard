#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>  // I/O 
#include <fstream>   // file I/O

#include "config.hpp"
#include "console.hpp"
#include"newsgroup.hpp"

#define CONFIGFILENAME "/.blackbeardrc"

using std::ofstream;
using std::ifstream;
using std::ios;
using std::endl;
using std::string;

// Public data members go here.
Config::Config(int argc, char *argv[]) // Constructor
{
    news_server = "localhost";
    news_group  = "alt.text";
    username="anonymous";
    password="guest";
    home_dir = getenv("HOME");
    blackbeard_dir = home_dir + "/blackbeard";
    blackbeard_data_dir = blackbeard_dir + "/.control";
    config_filename = home_dir + CONFIGFILENAME;
    news_port = 119;
    debug_mode = 0;
    load_file = "";
    show_groups_screen = 0;
    console->print_on_delete = 0;

    int ac=1;
    string debug = "-d";
    string load_headers = "-l";
    string load_groups = "-g";
    string show_groups_flag = "-G";
    string need_help_flag = "-h";

    while(ac < argc){
        if(0 == debug.compare(argv[ac])){
            console->print_logs = 1;
            console->keep_logs = 0;
            debug_mode = 1;
        }else if(0 == load_headers.compare(argv[ac])){
            ac++;
            load_file = argv[ac];
            console->log("will load postsets from file: " + load_file);
           // console->keep_logs = 0;
        }else if(0 == load_groups.compare(argv[ac])){
            ac++;
            load_group = argv[ac];
            console->log("Will load groups from file: " + load_groups);
      //      console->keep_logs = 0;
        }else if(0 == show_groups_flag.compare(argv[ac])){
            show_groups_screen = 1;
        }else if(0 == need_help_flag.compare(argv[ac])){
            console->print_logs = 1;
            console->keep_logs = 0;
            console->log("blb [-d] [-l filename] [-g filename] [-G] [-h]");
            console->log(" -d debug mode.  No ncurses, dumps log to STDOUT");
            console->log(" -l load subjects from filename, one per line");
            console->log(" -g load newsgroups from filename, one per line");
            console->log(" -G activate newsgroup browser mode");
            console->log(" -h This help");
            console->log("");
            exit(0);
        }

        ac++;
    }
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
    mkdir(blackbeard_data_dir.c_str(), 01777);

    ofstream out;
    out.open(config_filename.c_str(), ios::out);
    out << "#news_server=localhost" << endl;
    out << "#news_group=alt.text" << endl;
    out << "#news_port=119" << endl;
    out << "#username=anonymous" << endl;
    out << "#password=guest" << endl;
    out.close();
}

void Config::load_persistant_data(void)
{
    load_groups_from(blackbeard_data_dir + "/subscribed_newsgroups");
    Uint32 max_int = newsgroups.size();
    for(Uint32 i = 0 ; i < max_int ; i++)
    {
       newsgroups[i]->is_subscribed = 1;
       newsgroups[i]->load_postsets();
    }
}

// Private members go here.
// Protected members go here.
