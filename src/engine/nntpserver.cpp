#include<sstream>

#include "nntpserver.hpp"
#include "tcpconnection.hpp"
#include "news_constants.hpp"
#include "console.hpp"
#include "newsgrouppost.hpp"
#include "yenc_decoder.hpp"

#include"globals.hpp"

// Public data members go here.
NNTPServer::NNTPServer(std::string hostname, int port) : TCPConnection(hostname, port) // Constructor
{
    server_status = 0;
    read_packets();
    newsgroup->status = get_line();
    console->log(newsgroup->status);
}
    
NNTPServer::~NNTPServer() // Destructor
{
}

int NNTPServer::status()
{
    return server_status;
}

void NNTPServer::login(string username, std::string password)
{
    send_command("authinfo user " + username);
    send_command("authinfo pass " + password);
}

void NNTPServer::quit()
{
    send_command(QUIT);
}

void NNTPServer::group(string groupname)
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
    newsgroup->status = "Updating articles..";
    send_command(XOVER);

    read_xover_response();
    newsgroup->status = "Idle";
}

void NNTPServer::xover(long article_id)
{
    newsgroup->status = "Updating articles..";
    std::stringstream buf;
    buf << XOVER << article_id << "-";
    send_command(buf.str());

    read_xover_response();
    newsgroup->status = "Idle";
}

void NNTPServer::article(long article_id)
{
    std::stringstream buf;
    buf << ARTICLE << article_id;
    send_command(buf.str());

    read_multiline_response();
}

void NNTPServer::head(long article_id)
{
    std::stringstream buf;
    buf << HEAD << article_id;
    send_command(buf.str());

    read_multiline_response();
}

NewsGroupPost * NNTPServer::body(string article_id)
{
    newsgroup->status = "Fetching body " + article_id;
    std::stringstream buf;
    buf << BODY << article_id;
    send_command(buf.str());

    NewsGroupPost *post = read_body_response();
    newsgroup->status = "Idle";
    return post;
}

void NNTPServer::last()
{
    send_command(LAST);
}

void NNTPServer::help()
{
    send_command(HELP);
    read_multiline_response();
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

NewsGroupPost *NNTPServer::read_body_response()
{
    NewsGroupPost *newsgrouppost = new NewsGroupPost;
    int data_end = 0;
    while(data_end == 0){
        if(has_data_waiting()){
            std::string line = get_line();
            if((line.length() == 1 ) && (line[0] == '.')){
                data_end=1;
            }else{
                if(line[0] == '.'){
                    line.erase(0,1);
                }
                newsgrouppost->lines.push_back(line);
            }
        }else{
            read_packets();
        }
    }
    return newsgrouppost;
}

void NNTPServer::read_multiline_response()
{
    int data_end = 0;
    while(data_end == 0){
        if(has_data_waiting()){
            std::string line = get_line();
            //console->log("Considering (" + line + ")");
            if((line.length() == 1 ) && (line[0] == '.')){
                data_end=1;
              //  console->log("Found end of multi-line response");
            }else{
                if(line[0] == '.'){
                    line.erase(0,1);
                }
            }
        }else{
            read_packets();
        }
    }
}

void NNTPServer::read_xover_response()
{
    int data_end = 0;
    while(data_end == 0){
        if(has_data_waiting()){
            std::string line = get_line();
            if((line.length() == 1 ) && (line[0] == '.')){
                data_end=1;
            }else{
                if(line[0] == '.'){
                    line.erase(0,1);
                }
                newsgroup->header_scoop(line);
            }
        }else{
            read_packets();
        }
    }
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
    std::string response = get_line();
    console->log(response);
    console->log("[-<>-]");

    std::string server_response = response.substr(0, 3);
    if(0 == server_response.compare(AUTH_REQUIRED)){
        login(config->username, config->password);
        return send_command(command);
    }
}
// Private members go here.
// Protected members go here.
