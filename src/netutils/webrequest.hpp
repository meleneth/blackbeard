#ifndef ___webrequest_inc
#define ___webrequest_inc 1

#include "SDL_types.h"
#include "tcpconnection.hpp"
#include <string>

using std::string;

class WebRequest {
    public:
        WebRequest(TCPConnection *client);
        ~WebRequest();

        TCPConnection *client;
        string request_string;
        string path;
        string filename;

    private:
    protected:
};

#endif
