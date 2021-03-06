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
#ifdef __WIN32__
#include <winsock2.h>
#include <wininet.h>
#else
#include <sys/socket.h>
#include<sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include<time.h>
#endif

#define MAXDATASIZE 960000

using std::list;
using std::string;

class TCPConnection {
    public:
        // Public data members go here.
        TCPConnection(string hostname, int port); // Constructor
        TCPConnection(); // Constructor
        virtual ~TCPConnection(); // Destructor

        void send_data(char *buf, Uint32 size);
        int has_data_waiting(void);
        void send_line(string line);
        void slice_buffer_strings(void);
        Uint32 read_packets(void);
        Uint32 num_ticks(struct timeval *now);
        virtual void send_command(string command);
        void sendall(string cmd);
        void close_connection(void);
        Uint32 get_krate(void);
        std::string get_line(void);

        int sockfd, numbytes;  
        char buf[MAXDATASIZE];
        int buf_start_pos, buf_end_pos;
        struct hostent *he;
        struct sockaddr_in their_addr; // connector's address information 
        list<string> lines;
        int connected;
        struct timeval last_time;
        Uint32 bytes_since_last_time;
        Uint32 krate;
        time_t last_activity_at;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
