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
    gettimeofday(&last_time, NULL);
    bytes_since_last_time = 0;
}

TCPConnection::TCPConnection()
{
    gettimeofday(&last_time, NULL);
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
        if(n == 0) { connected = 0; total = num_bytes + 1; }
        total += n;
        bytesleft -= n;
    }
}

void TCPConnection::send_data(char *buf, Uint32 size)
{
    if(!connected)
        return;
    int total = 0;        // how many bytes we've sent
    int bytesleft = size; // how many we have left to send
    int num_bytes = bytesleft;
    int n;

    while(total < num_bytes) {
        n = send(sockfd, buf+total, bytesleft, 0);
        if (n == -1) { console->log("Network error on send?  Oh I'm so scared :/"); break; }
        total += n;
        bytesleft -= n;
    }
}

Uint32 TCPConnection::read_packets(void)
{
    // will only be called when we have data waiting
    if(!connected)
        return 0;

    if ((numbytes=recv(sockfd, buf + buf_end_pos, MAXDATASIZE - buf_end_pos, 0)) == -1) {
        connected = 0;
        return 0;
    }
    if(numbytes == 0) { connected = 0; return 0;}
    bytes_since_last_time +=  numbytes;

    buf_end_pos += numbytes;
    slice_buffer_strings();
    return numbytes;
}

Uint32 TCPConnection::get_krate()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    Uint32 ticks = num_ticks(&now);
    // 10 ticks / second for krate
    if(ticks){
        if(bytes_since_last_time){
            krate = (krate + 
                    ((bytes_since_last_time / ticks)/(1024/10))
                    )/2 ;
            bytes_since_last_time = 0;
        }else{
            krate = 0;
        }
        last_time.tv_usec = now.tv_usec;
        last_time.tv_sec = now.tv_sec;
    }

    return krate;
}

#define MS_PER_TICK 100000

Uint32 TCPConnection::num_ticks(struct timeval *now)
{   
    return ((now->tv_sec - last_time.tv_sec) * 10)
           + (now->tv_usec / MS_PER_TICK) 
           - (last_time.tv_usec / MS_PER_TICK);
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
