#ifndef ___nntpserver_inc
#define ___nntpserver_inc 1

#include"SDL_types.h"

#include "tcpconnection.hpp"
#include<string>
#include<list>

class NNTPServer : public TCPConnection {
    public:
        // Public data members go here.
        NNTPServer(std::string hostname, int port); // Constructor
        ~NNTPServer(); // Destructor

        virtual void send_command(std::string cmd);

       int status();
       void login(std::string username, std::string password);
       void quit();
       void group(std::string groupname);
       void list();
       void listgroup();
       void xover();
       void xover(long article_id);
       void xover_format();
       void article(long article_id);
       void head(long article_id);
       void body(long article_id);
       void last();
       void help();
       void date();
       void next();
       void post();
       void stat();

	int server_status;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
