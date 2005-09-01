#ifndef ___tcpconnection_inc
#define ___tcpconnection_inc 1

#include"SDL_types.h"

#include<string>
#include<list>

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

#define MAXDATASIZE 960000

using std::list;
using std::string;

class TCPConnection {
    public:
        // Public data members go here.
        TCPConnection(string hostname, int port); // Constructor
        virtual ~TCPConnection(); // Destructor

        int has_data_waiting(void);
        void send_line(string line);
        void slice_buffer_strings(void);
        void read_packets(void);
        virtual void send_command(string command);
        void sendall(string cmd);
        std::string get_line(void);

        int sockfd, numbytes;  
        char buf[MAXDATASIZE];
        int buf_start_pos, buf_end_pos;
        struct hostent *he;
        struct sockaddr_in their_addr; // connector's address information 
        list<string> lines;
        int connected;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
