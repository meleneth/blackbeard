#ifndef ___nntpserver_inc
#define ___nntpserver_inc 1

#include"SDL.h"

#include "tcpconnection.hpp"
#include<string>
#include<list>

class NNTPServer : public TCPConnection {
    public:
        // Public data members go here.
        NNTPServer::NNTPServer(std::string hostname, int port); // Constructor
        NNTPServer::~NNTPServer(); // Destructor

       int NNTPServer::status();
       void NNTPServer::login(std::string username, std::string password);
       void NNTPServer::quit();
       void NNTPServer::group(std::string groupname);
       void NNTPServer::list();
       void NNTPServer::listgroup();
       void NNTPServer::xover();
       void NNTPServer::xover(long article_id);
       void NNTPServer::xover_format();
       void NNTPServer::article(long article_id);
       void NNTPServer::head(long article_id);
       void NNTPServer::body(long article_id);
       void NNTPServer::last();
       void NNTPServer::help();
       void NNTPServer::date();
       void NNTPServer::next();
       void NNTPServer::post();
       void NNTPServer::stat();

	int server_status;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
