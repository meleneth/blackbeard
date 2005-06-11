#ifndef ___tcpconnection_inc
#define ___tcpconnection_inc 1

#include"SDL.h"

#include<string>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAXDATASIZE 9000

class TCPConnection {
    public:
        // Public data members go here.
        TCPConnection::TCPConnection(std::string hostname, int port); // Constructor
        TCPConnection::~TCPConnection(); // Destructor
        int Foo(void);

        int sockfd, numbytes;  
        char buf[MAXDATASIZE];
        struct hostent *he;
        struct sockaddr_in their_addr; // connector's address information 
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
