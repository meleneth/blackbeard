#include "tcpconnection.hpp"
#include "console.hpp"

// Public data members go here.
TCPConnection::TCPConnection(std::string hostname, int port) // Constructor
{

    if ((he=gethostbyname(hostname.c_str())) == NULL) {  // get the host info 
        perror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    their_addr.sin_family = AF_INET;    // host byte order 
    their_addr.sin_port = htons(port);  // short, network byte order 
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(their_addr.sin_zero), '\0', 8);  // zero the rest of the struct 

    if (connect(sockfd, (struct sockaddr *)&their_addr,
                                          sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }

}


    
TCPConnection::~TCPConnection() // Destructor
{
    close(sockfd);
}

int TCPConnection::has_data_waiting(void)
{
    if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
    buf[numbytes] = '\0';
    console->log(buf);
}

std::string TCPConnection::get_line(void)
{
    if(!has_data_waiting()){
        return NULL;
    }
    return "moo";
}

// Private members go here.
// Protected members go here.
