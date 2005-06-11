#ifndef ___tcpconnection_inc
#define ___tcpconnection_inc 1

#include"SDL.h"

class TCPConnection {
    public:
        // Public data members go here.
        TCPConnection::TCPConnection(int bar); // Constructor
        TCPConnection::~TCPConnection(); // Destructor
        int Foo(void);
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
