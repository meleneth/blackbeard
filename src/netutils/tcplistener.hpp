#ifndef ___tcplistener_inc
#define ___tcplistener_inc 1

#include "SDL_types.h"
#include "tcpconnection.hpp"

class TCPListener {
    public:
        TCPListener(int port_num);
        ~TCPListener();

        TCPConnection *get_waiting_connection();
        int has_connection_waiting();
        
        int port_no;

    private:
    protected:
};

#endif
