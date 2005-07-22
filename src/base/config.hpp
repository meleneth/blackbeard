#ifndef ___config_inc
#define ___config_inc 1

#include"SDL_types.h"
#include<string>

using std::string;

class Config {
    public:
        // Public data members go here.
        Config(int argc, char *argv[]); // Constructor
        ~Config(); // Destructor

        void read_config_file(void);
        void setup_files(void);

        string news_server;
        string news_group;
        string home_dir;
        string blackbeard_dir;
        string config_filename;
        string load_file;
        string load_group;
        string blackbeard_data_dir;
        string username;
        string password;
        Uint32 debug_mode;
        Uint32 show_groups_screen;

        Uint32 news_port;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
