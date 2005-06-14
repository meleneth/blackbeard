#include "nntpserver.hpp"
#include "tcpconnection.hpp"
#include "news_constants.hpp"
#include "console.hpp"

// Public data members go here.
NNTPServer::NNTPServer(std::string hostname, int port) : TCPConnection(hostname, port) // Constructor
{
}
    
NNTPServer::~NNTPServer() // Destructor
{
}

int
NNTPServer::status()
{
    return server_status;
}

bool
NNTPServer::login(std::string username, std::string password)
{
    send_command("authinfo user arnuga");
    std::string response = get_line();
    console->log(">> " + response);
    send_command("authinfo pass leper56");

    return true;
}

bool
NNTPServer::quit()
{
    send_command(QUIT);
    return true;
}

// Private members go here.
// Protected members go here.
