#include<sstream>

#include "nntpserver.hpp"
#include "tcpconnection.hpp"
#include "news_constants.hpp"
#include "console.hpp"

#include"globals.hpp"

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
    send_command("authinfo user " + username);
    send_command("authinfo pass " + password);
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

    int data_end = 0;
    while(data_end = 0){
        if(has_data_waiting()){
            std::string line = get_line();
            if(line.length() == 4 && line[0] == '.')
                data_end=1;
        }else{
            read_packets();
        }
    }

}

void NNTPServer::xover(long article_id)
{
    std::stringstream buf;
    buf << XOVER << article_id << "-";
    send_command(buf.str());
}

void NNTPServer::article(long article_id)
{
    std::stringstream buf;
    buf << ARTICLE << article_id;
    send_command(buf.str());
}

void NNTPServer::head(long article_id)
{
    std::stringstream buf;
    buf << HEAD << article_id;
    send_command(buf.str());
}

void NNTPServer::body(long article_id)
{
    std::stringstream buf;
    buf << BODY << article_id;
    send_command(buf.str());
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

void NNTPServer::send_command(std::string command)
{
    if(has_data_waiting()){
        console->log("It is an error to try to send a command with the following data ready to be read:");
        console->log(get_line());
        shut_down();
    }

    console->log("Sending command: " + command);
    TCPConnection::send_command(command);

    read_packets();
    while(has_data_waiting()){
        std::string response = get_line();
        console->log(response);
    }
    console->log("[-<>-]");
}
// Private members go here.
// Protected members go here.
