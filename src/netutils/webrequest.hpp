#ifndef ___webrequest_inc
#define ___webrequest_inc 1

#include "SDL_types.h"
#include "tcpconnection.hpp"
#include <string>

using std::string;

class WebRequest {
    public:
        WebRequest(TCPConnection *client);
        WebRequest(string uri);
        ~WebRequest();

        void parse_uri(string uri);
        void parse_headers(void);
        void split_request_uri(string uri);

        TCPConnection *client;
        string request_string;
        string path;
        string filename;
        Uint32 http_minor_version;
        Uint32 http_major_version;

    private:
    protected:
};

#endif
