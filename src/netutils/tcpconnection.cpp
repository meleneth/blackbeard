#include "tcpconnection.hpp"
#include "console.hpp"

// Public data members go here.
TCPConnection::TCPConnection(string hostname, int port) // Constructor
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
    connected = 1;
    last_second = time(NULL);
}

TCPConnection::TCPConnection()
{
    last_second = time(NULL);
    connected = 0;
    buf_start_pos = 0;
    buf_end_pos = 0;
}
    
TCPConnection::~TCPConnection() // Destructor
{
    close_connection();
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
            string line = buf + buf_start_pos;
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

    int num_bytes = buf_end_pos - buf_start_pos;
    memmove(buf, buf + buf_start_pos, num_bytes);
    buf_end_pos = num_bytes;
    buf_start_pos = 0;
}

void TCPConnection::send_command(string command)
{
    command += "\r\n";
    sendall(command);
}

void TCPConnection::sendall(string cmd)
{
    if(!connected)
        return;
    int total = 0;        // how many bytes we've sent
    int bytesleft = cmd.length(); // how many we have left to send
    int num_bytes = bytesleft;
    int n;
    const char *nbuf = cmd.c_str();

    while(total < num_bytes) {
        n = send(sockfd, nbuf+total, bytesleft, 0);
        if (n == -1) { console->log("Network error on send?  Oh I'm so scared :/"); break; }
        total += n;
        bytesleft -= n;
    }
}

void TCPConnection::read_packets(void)
{
    // will only be called when we have data waiting
    if(!connected)
        return;

    if ((numbytes=recv(sockfd, buf + buf_end_pos, MAXDATASIZE - buf_end_pos, 0)) == -1) {
        connected = 0;
        return;
    }

    bytes_since_last_second +=  numbytes;
    time_t now = time(NULL);
    if(now != last_second){
        if(bytes_since_last_second){
            krate = (bytes_since_last_second / (now - last_second))/1024;
            bytes_since_last_second = 0;
        }else{
            krate = 0;
        }
        last_second = now;
    }
    buf_end_pos += numbytes;
    slice_buffer_strings();
}

string TCPConnection::get_line(void)
{
    if(!has_data_waiting()){
        return NULL;
    }
    string line = *lines.begin();
    lines.pop_front();
    
    return line;
}

void TCPConnection::close_connection(void)
{
    if(!connected)
        return;
    connected = 0;

    close(sockfd);
}

// Private members go here.
// Protected members go here.
