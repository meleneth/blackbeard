#include "tcpconnection.hpp"
#include "console.hpp"

#include<pthread.h>

// Public data members go here.
TCPConnection::TCPConnection(std::string hostname, int port) // Constructor
{
    buf_start_pos = 0;
    buf_end_pos = 0;

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

    if ( connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) 
         == -1) {
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
    return !lines.empty();
}

void TCPConnection::slice_buffer_strings(void)
{
    int index;

    index = buf_start_pos;
    while(index != buf_end_pos){
        if(buf[index] == '\r')
            buf[index] = 0;
        if(buf[index] == '\n'){
            buf[index] = 0;
            std::string line = buf + buf_start_pos;
            lines.push_back(line);
            buf_start_pos = index+1;
            if(buf_start_pos==buf_end_pos){
                buf_start_pos = 0;
                buf_end_pos = 0;
                index = -1;
                break;
            }
        }
        index++;
    }

}

void TCPConnection::send_command(std::string command)
{
    command += "\r\n";
    sendall(command);
}

void TCPConnection::sendall(std::string cmd)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = cmd.length(); // how many we have left to send
    int num_bytes = bytesleft;
    int n;
    const char *nbuf = cmd.c_str();

    while(total < num_bytes) {
        n = send(sockfd, nbuf+total, bytesleft, 0);
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }
}

void TCPConnection::read_packets(void)
{
    // will only be called when we have data waiting
    console->log("Calling recv()");
    if ((numbytes=recv(sockfd, buf + buf_end_pos, MAXDATASIZE - buf_end_pos, 0)) == -1) {
        perror("recv");
        exit(1);
    }
    console->log("done with  recv()");
    buf_end_pos += numbytes;
    slice_buffer_strings();
}

std::string TCPConnection::get_line(void)
{
    if(!has_data_waiting()){
        return NULL;
    }
    std::string line = *lines.begin();
    lines.pop_front();
    return line;
}

// Private members go here.
// Protected members go here.
