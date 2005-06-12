#include "netcentral.hpp"
#include"console.hpp"

// Public data members go here.
NetCentral::NetCentral(void) // Constructor
{
    FD_ZERO(&master);    // clear the master and temp sets
    fdmax = 0;
    console->log("NetCentral() initialized");
}
    
NetCentral::~NetCentral() // Destructor
{
}

void NetCentral::tick(void)
{
    if(connections.size() == 0){
        return;
    }
    std::list<TCPConnection *>::iterator i;
    FD_ZERO(&read_fds);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    fdmax = 0;

    for(i = connections.begin(); i != connections.end(); ++i){
        FD_SET((*i)->sockfd, &read_fds);
        if(fdmax < (*i)->sockfd){
            fdmax = (*i)->sockfd;
        }
    }

    if (select(fdmax+1, &read_fds, NULL, NULL, &tv) == -1) {
        perror("select");
        exit(1);
    }

    for(i = connections.begin(); i != connections.end(); ++i){
        if(FD_ISSET((*i)->sockfd, &read_fds)){
            (*i)->read_packets();
        }

        if((*i)->has_data_waiting()){
            console->log((*i)->get_line());
        }
    }
}

void NetCentral::add_connection(TCPConnection *connection)
{
    connections.push_front(connection);
    if(fdmax < connection->sockfd){
        fdmax = connection->sockfd;
    }
    FD_SET(connection->sockfd, &master);
}

// Private members go here.
// Protected members go here.
