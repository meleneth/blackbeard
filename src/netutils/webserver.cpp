#include "webserver.hpp"
#include "webrequest.hpp"
#include "console.hpp"
#include "file_handle.hpp"
#include "webfilefetcher.hpp"

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
   string filename = web_root + request->path + request->filename;
   console->log("Sending file: " + filename);

   handlers.push_back(new WebFileFetcher(request, filename));
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

    list<WebDataFetcher *>::iterator h;

    for(h = handlers.begin(); h != handlers.end() ; ++h) {
        if (!(*h)->tick()) {
            list<WebDataFetcher *>::iterator j = h;
            WebDataFetcher *r = (*h);
            --h;
            handlers.erase(j);
            delete r;
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
