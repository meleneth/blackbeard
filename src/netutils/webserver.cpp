#include "webserver.hpp"
#include "webrequest.hpp"
#include "console.hpp"

WebServer::WebServer(string web_root, int port_no)
{
    listener = new TCPListener(port_no);
    this->web_root = web_root;
}

WebServer::~WebServer()
{
    delete listener;
}

void WebServer::handle_request(WebRequest *request)
{
   console->log("Sending file: " + web_root + request->path + request->filename);
}

void WebServer::tick(void)
{
    list <TCPConnection *>::iterator i;

    for(i = connections.begin(); i != connections.end(); ++i){
        if((*i)->has_data_waiting()){
            handle_request(new WebRequest(*i));
            list <TCPConnection *>::iterator p = i;
            --i;
            connections.erase(p);
        }
    }
}

void WebServer::log_to_file(string filename)
{
}

void WebServer::handle_new_connection(void)
{
    connections.push_back(listener->get_waiting_connection());
}
