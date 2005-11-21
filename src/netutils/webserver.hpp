#ifndef ___webserver_inc
#define ___webserver_inc 1

#include "SDL_types.h"
#include <string>
#include <list>
#include "tcplistener.hpp"
#include "webrequest.hpp"

using std::string;
using std::list;

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

    private:
    protected:
};

extern WebServer *webserver;

#endif
