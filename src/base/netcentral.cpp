#include "netcentral.hpp"
#include"console.hpp"
#include"config.hpp"

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

void NetCentral::process_jobs(void)
{
    if(connections.size() == 0){
        return;
    }
    FD_ZERO(&read_fds);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 10;
    fdmax = 0;

    Uint32 max_con = connections.size();
    Uint32 i;

    for(i = 0; i<max_con; ++i){
        FD_SET(connections[i]->sockfd, &read_fds);
        if(fdmax < connections[i]->sockfd){
            fdmax = connections[i]->sockfd;
        }
    }

    if (select(fdmax+1, &read_fds, NULL, NULL, &tv) == -1) {
        perror("select");
        exit(1);
    }

//    if(jobs.size()){
//        if(active_jobs.size() < config->max_net_connections){
//            get_next_job();
//        }    
//    }

    for(i = 0; i<max_con; ++i){
        if(FD_ISSET(connections[i]->sockfd, &read_fds)){
            connections[i]->read_packets();
        }

        if(connections[i]->has_data_waiting()){
            console->log(connections[i]->get_line());
        }
    }
}

Job *NetCentral::get_next_job(void)
{
    if(config->max_net_connections == active_jobs.size()){
        vector<Job *>::iterator j;
        Job * job;
        
        j = jobs.begin();
        job = *j;
        jobs.erase(j);
        active_jobs.push_back(job);
        return job;
    }
    return NULL;
}

void NetCentral::add_connection(NNTPServer *connection)
{
    connections.push_back(connection);
    if(fdmax < connection->sockfd){
        fdmax = connection->sockfd;
    }
    FD_SET(connection->sockfd, &master);
}

// Private members go here.
// Protected members go here.
