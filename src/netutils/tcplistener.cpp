#include "tcplistener.hpp"
#include "console.hpp"

#define BACKLOG 4

TCPListener::TCPListener(int port_num)
{
    port_no = port_num;
    yes=1;

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    
    my_addr.sin_family = AF_INET;         // host byte order
    my_addr.sin_port = htons(port_no);     // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(&(my_addr.sin_zero), '\0', 8); // zero the rest of the struct

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))
                                                                   == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    console->log("Listener listening..");
}

TCPListener::~TCPListener()
{
}

TCPConnection *TCPListener::get_waiting_connection()
{
    console->log("get_waiting_connection...");
    
    TCPConnection *c = new TCPConnection();
    c->connected = 1;
    socklen_t sin_size;
    sin_size = sizeof(c->their_addr);
    c->sockfd = accept(sockfd, (struct sockaddr *)&c->their_addr, &sin_size);
    return c;
}

