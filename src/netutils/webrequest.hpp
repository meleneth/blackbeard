#ifndef ___webrequest_inc
#define ___webrequest_inc 1

#include "SDL_types.h"
#include "tcpconnection.hpp"

class WebRequest {
    public:
        WebRequest(TCPConnection *client);
        ~WebRequest();

        TCPConnection *client;

    private:
    protected:
};

#endif
