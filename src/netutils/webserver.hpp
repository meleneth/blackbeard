#ifndef ___webserver_inc
#define ___webserver_inc 1

#include "SDL_types.h"
#include <string>
#include <list>
#include "tcplistener.hpp"
#include "webrequest.hpp"
#include "webdatafetcher.hpp"

using std::string;
using std::list;

// This entire webserver is such a non-standard-using
// non RFC piece of work, that this program shouldn't
// even be using it.

class WebServer {
    public:
        WebServer(string document_root, int port_no);
        ~WebServer();

        void log_to_file(string filename);
        void handle_request(WebRequest *request);
        void handle_new_connection(void);
        void tick(void);

        TCPListener *listener;
        list<TCPConnection *> connections;
        list<WebDataFetcher *> handlers;
        string web_root;


    private:
    protected:
};

extern WebServer *webserver;

#endif
