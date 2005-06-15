#include "nntpserver.hpp"
#include "tcpconnection.hpp"
#include "news_constants.hpp"
#include "console.hpp"

// Public data members go here.
NNTPServer::NNTPServer(std::string hostname, int port) : TCPConnection(hostname, port) // Constructor
{
    server_status = 0;
}
    
NNTPServer::~NNTPServer() // Destructor
{
}

int NNTPServer::status()
{
    return server_status;
}

void NNTPServer::login(std::string username, std::string password)
{
    send_command("authinfo user arnuga");
    send_command("authinfo pass leper56");
    send_command(MODE_READER);
}

void NNTPServer::quit()
{
    send_command(QUIT);
}

void NNTPServer::group(std::string groupname)
{
    send_command(GROUP + groupname);
}

void NNTPServer::list()
{
    send_command(LIST);
}

void NNTPServer::listgroup()
{
    send_command(LISTGROUP);
}

void NNTPServer::xover()
{
    send_command(XOVER);
}

void NNTPServer::xover(long article_id)
{
    send_command(XOVER + article_id + '-');
}

void NNTPServer::article(long article_id)
{
    send_command(ARTICLE + article_id);
}

void NNTPServer::head(long article_id)
{
    send_command(HEAD + article_id);
}

void NNTPServer::body(long article_id)
{
    send_command(BODY + article_id);
}

void NNTPServer::last()
{
    send_command(LAST);
}

void NNTPServer::help()
{
    send_command(HELP);
}

void NNTPServer::date()
{
    send_command(DATE);
}

void NNTPServer::next()
{
    send_command(NEXT);
}

void NNTPServer::post()
{
}

void NNTPServer::stat()
{
    send_command(STAT);
}

void NNTPServer::xover_format()
{
    send_command(OVERVIEW_FMT);
}
// Private members go here.
// Protected members go here.
