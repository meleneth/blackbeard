#ifndef ___webrequest_inc
#define ___webrequest_inc 1

#include "SDL_types.h"
#include "tcpconnection.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;

class WebRequest {
    public:
        WebRequest(TCPConnection *client);
        WebRequest(string uri);
        ~WebRequest();

        void parse_uri(string uri);
        void parse_headers(void);
        void split_request_uri(string uri);
        void defaults(void);
        string param(string name);

        TCPConnection *client;
        string request_string;
        string path;
        string filename;
        Uint32 http_minor_version;
        Uint32 http_major_version;
        Uint32 has_cgi_params;

        vector<string> param_names;
        vector<string> param_values;

    private:
    protected:
};

#endif
