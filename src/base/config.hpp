#ifndef ___config_inc
#define ___config_inc 1

#include "SDL_types.h"
#include <string>

using std::string;

// Zen: the Config object should be *driving* the
// configuration of the objects of the program, and
// therefore be last instantiated.

class Config {
    public:
        // Public data members go here.
        Config(int argc, char *argv[]); // Constructor
        ~Config(); // Destructor

        void read_config_file(void);
        void setup_files(void);
        void set_base_dir(string path);
        void load_persistant_data(void);
        void setup_test_config();
        void save_persistent_data(void);

        string newsgroups_xml_filename(void);

        string subscribed_groups_filename();
        string net_jobs_filename();
        string full_job_filename(string job_filename);
        string downloaded_postsets_filename();
        string newsgroup_postsets_filename();

        string news_server;
        string home_dir;
        string blackbeard_dir;
        string config_filename;
        string load_file;
        string load_group;
        string blackbeard_data_dir;
        string username;
        string password;
        Uint32 news_port;
        Uint32 webserver_port;
        Uint32 debug_mode;
        Uint32 test_mode;
        Uint32 show_groups_screen;
        Uint32 max_net_connections;
        Uint32 debug_logging;
        Uint32 dump_subject_file;
        Uint32 max_krate;

        Uint32 tick;
        Uint32 proceed;

    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

extern Config *config;

#endif
