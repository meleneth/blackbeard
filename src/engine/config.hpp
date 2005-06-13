#ifndef ___config_inc
#define ___config_inc 1

#include"SDL.h"
#include<string>

class Config {
    public:
        // Public data members go here.
        Config(); // Constructor
        ~Config(); // Destructor

        void read_config_file(void);
        void setup_files(void);

        std::string news_server;
        std::string newsgroup;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
