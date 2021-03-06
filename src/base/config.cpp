#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>  // I/O 
#include <fstream>   // file I/O

#include "config.hpp"
#include "console.hpp"
#include "newsgroup.hpp"
#include "netcentral.hpp"
#include "database.hpp"

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
    username="anonymous";
    password="guest";
    home_dir = getenv("HOME");
    set_base_dir(home_dir + "/blackbeard");
    config_filename = home_dir + CONFIGFILENAME;
    news_port = 119;
    debug_mode = 0;
    max_krate = 400;
    load_file = "";
    webserver_port = 4242;
    show_groups_screen = 0;
    console->print_on_delete = 0;
    max_net_connections = 2;
    debug_logging = 0;
    dump_subject_file = 0;
    test_mode = 0;
    proceed = 1;
    tick = 0;

    int ac=1;
    string debug = "-d";
    string save_console_to_file_flag = "-f";
    string load_headers = "-l";
    string load_groups = "-g";
    string show_groups_flag = "-G";
    string need_help_flag = "-h";
    string dump_subjects_flag = "-s";

    while(ac < argc){
        if(0 == debug.compare(argv[ac])){
            console->print_logs = 1;
            console->keep_logs = 0;
            debug_mode = 1;
            debug_logging =1;
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
        }else if(0 == dump_subjects_flag.compare(argv[ac])){
            dump_subject_file = 1;
        }else if(0 == save_console_to_file_flag.compare(argv[ac])){
            console->open_log_file();
        }else if(0 == need_help_flag.compare(argv[ac])){
            console->print_logs = 1;
            console->keep_logs = 0;
            console->log("blb [-d] [-l filename] [-g filename] [-G] [-f] [-h]");
            console->log(" -d debug mode.  No ncurses, dumps log to STDOUT");
            console->log(" -l load full headers from filename, one per line");
            console->log(" -G activate newsgroup browser mode");
            console->log(" -f save console logs to console.log");
            console->log(" -s save full headers to headers.log instead of processing");
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

void Config::set_base_dir(string dirname)
{
    console->log("Setting base dir name to [" + dirname + "]");
    blackbeard_dir = dirname;
    blackbeard_data_dir = blackbeard_dir + "/.control";
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
            } else if(0 == cmd.compare("news_port")){
                news_port = atoi(value.c_str());
            } else if(0 == cmd.compare("max_krate")){
                max_krate = atoi(value.c_str());
            } else if(0 == cmd.compare("username")){
                username = value;
            } else if(0 == cmd.compare("password")){
                password = value;
            } else if(0 == cmd.compare("data_dir")){
                set_base_dir(value);
            } else if(0 == cmd.compare("max_net_connections")){
                max_net_connections = atoi(value.c_str());
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
#ifdef __WIN32__
    mkdir(blackbeard_dir.c_str());
    mkdir(blackbeard_data_dir.c_str());
#else
    mkdir(blackbeard_dir.c_str(), 01777);
    mkdir(blackbeard_data_dir.c_str(), 01777);
#endif

    ofstream out;
    out.open(config_filename.c_str(), ios::out);
    out << "#news_server=localhost" << endl;
    out << "#news_port=119" << endl;
    out << "#username=anonymous" << endl;
    out << "#max_krate=400" << endl;
    out << "#password=guest" << endl;
    out << "#max_net_connections=4" << endl << endl;
    out << "# if data dir is not set it defaults to ~/blackbeard" << endl;
    out << "#data_dir=/mnt/blackbeard" << endl;
    out.close();
}

void Config::load_persistant_data(void)
{
    console->log("loading persistant data..");
    restore_newsgroups();
}

void Config::save_persistent_data(void)
{
    console->log("saving persistant data..");
    save_subscribed_groups();
}

void Config::setup_test_config(void)
{
    test_mode = 1;
}

string Config::subscribed_groups_filename()
{
    return blackbeard_data_dir + "/subscribed_newsgroups";
}

string Config::full_job_filename(string job_filename)
{
    return blackbeard_data_dir + "/" + job_filename;
}

string Config::newsgroups_xml_filename(void)
{
    return blackbeard_data_dir + "/newsgroups.xml";
}

string Config::newsgroup_postsets_filename()
{
    return blackbeard_data_dir;
}


// Private members go here.
// Protected members go here.
