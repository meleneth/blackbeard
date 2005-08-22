#include<sstream>

#include "nntpserver.hpp"
#include "tcpconnection.hpp"
#include "news_constants.hpp"
#include "console.hpp"
#include "config.hpp"
#include "newsgrouppost.hpp"
#include "yenc_decoder.hpp"

using std::string;
using std::stringstream;

extern void shut_down(void);

// Public data members go here.
NNTPServer::NNTPServer(string hostname, int port) : TCPConnection(hostname, port) // Constructor
{
    newsgroup = NULL;
    is_multiline_reading = 0;
    _status = NS_COMMAND_RESPONSE;
}
    
NNTPServer::~NNTPServer() // Destructor
{
}

int NNTPServer::status()
{
}

void NNTPServer::tick(void)
{
    switch(_status){
        case NS_IDLE:
            break;
        case NS_CONNECTED:
            break;
        case NS_LOGIN:
            if(has_data_waiting()){
                string line = get_line();
                TCPConnection::send_command("authinfo pass " + _password);
                _password = "";
                _status = NS_PASSWORD;
            }
            break;
        case NS_PASSWORD:
            if(has_data_waiting()){
                string line = get_line();
                _status = NS_CONNECTED;
                if(_current_command.compare("")){
                    send_command(_current_command);
                }
            }
            break;
        case NS_COMMAND_RESPONSE:
            if(has_data_waiting()){
                string line = get_line();
                string server_response = line.substr(0, 3);
                _status = NS_CONNECTED;
                if(0 == server_response.compare(AUTH_REQUIRED)){
                    login(config->username, config->password);
                    break;
                }
                _current_command = "";
            }
            break;
    }
}

void NNTPServer::login(string username, string password)
{
    send_command("authinfo user " + username);
    _status = NS_LOGIN;
    _password = password;
}

void NNTPServer::quit()
{
    send_command(QUIT);
}

void NNTPServer::group(string groupname)
{
    send_command(GROUP + groupname);
    newsgroup = group_for_name(groupname);
}

vector<NewsGroup *> NNTPServer::list()
{
    send_command(LIST);
    is_multiline_reading = 1;
}

void NNTPServer::listgroup()
{
    send_command(LISTGROUP);
}

void NNTPServer::xover()
{
//    newsgroup->status = "Updating articles..";
    send_command(XOVER);

    read_xover_response();
//    newsgroup->status = "Idle";
}

void NNTPServer::xover(long article_id)
{
 //   newsgroup->status = "Updating articles..";
    stringstream buf;
    buf << XOVER << article_id << "-";
    send_command(buf.str());

    read_xover_response();
    newsgroup->status = "Idle";
}

void NNTPServer::article(long article_id)
{
    stringstream buf;
    buf << ARTICLE << article_id;
    send_command(buf.str());

    read_multiline_response();
}

void NNTPServer::head(long article_id)
{
    stringstream buf;
    buf << HEAD << article_id;
    send_command(buf.str());

    read_multiline_response();
}

NewsGroupPost * NNTPServer::body(string article_id)
{
   // newsgroup->status = "Fetching body " + article_id;
    stringstream buf;
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
    NewsGroupPost *newsgrouppost = new NewsGroupPost();
    string line = get_next_multi_line();
    while(is_multiline_reading) {
        newsgrouppost->lines.push_back(line);
        line = get_next_multi_line();
    }
    return newsgrouppost;
}

void NNTPServer::read_multiline_response()
{
    string line = get_next_multi_line();
    while(is_multiline_reading) {
        line = get_next_multi_line();
    }
}

void NNTPServer::read_xover_response()
{
    string line = get_next_multi_line();
    while(is_multiline_reading) {
        newsgroup->header_scoop(line);
        line = get_next_multi_line();
    }
}

void NNTPServer::send_command(string command)
{
    if(has_data_waiting()){
        console->log("It is an error to try to send a command with the following data ready to be read:");
        console->log(get_line());
        shut_down();
    }

    if(_status != NS_CONNECTED){
        console->log("Can't send command(" + command + ") unless status is NS_CONNECTED");
        shut_down();
    }

    console->log("Sending command: " + command);
    _current_command = command;
    TCPConnection::send_command(command);
    _status = NS_COMMAND_RESPONSE;
}

string NNTPServer::get_next_multi_line(void)
{
    if(!is_multiline_reading){
        return "";
    }

    if(!has_data_waiting()){
        return "";
    }

    string line = get_line();
    if((line.length() == 1 ) && (line[0] == '.')){
        is_multiline_reading = 0;
        return "";
    }

    if(line[0] == '.'){
        line.erase(0,1);
    }

    return line;
}

