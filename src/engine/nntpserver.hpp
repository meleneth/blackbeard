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
       bool NNTPServer::login(std::string username, std::string password);
       bool NNTPServer::quit();
       std::string NNTPServer::group(std::string groupname);
       std::list<std::string> NNTPServer::xover();

	int server_status;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
