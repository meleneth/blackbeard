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
    if((0 == jobs.size()) && (0 == active_jobs.size())){
        usleep(10);
        return;
    }

    FD_ZERO(&read_fds);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 10;
    fdmax = 0;

    Uint32 max_jobid = active_jobs.size();
    Uint32 i;

    for(i = 0; i<max_jobid; ++i){
        NNTPServer *connection = (NNTPServer *) active_jobs[i]->srv;
        FD_SET(connection->sockfd, &read_fds);
        if(fdmax < connection->sockfd){
            fdmax = connection->sockfd;
        }
    }

    if (select(fdmax+1, &read_fds, NULL, NULL, &tv) == -1) {
        perror("select");
        exit(1);
    }

    if(jobs.size()){
        if(active_jobs.size() < config->max_net_connections){
            Job *new_job = get_next_job();
            if(new_job){
                new_job->srv = new NNTPServer(config->news_server, config->news_port);
                active_jobs.push_back(new_job);
                max_jobid = active_jobs.size();
            }
        }    
    }
    
    for(i = 0; i<max_jobid; ++i){
        Job *job = active_jobs[i];
        NNTPServer *connection = (NNTPServer *) job->srv;

        if(FD_ISSET(connection->sockfd, &read_fds)){
            connection->read_packets();
        }

        connection->tick();
        job->process();

        if(job->is_finished){
            Job *new_job = get_next_job();
            if(new_job){
                new_job->srv = job->srv;
                active_jobs[i] = new_job;
            }else{
                active_jobs.erase(active_jobs.begin() + i);
                max_jobid = active_jobs.size();
                i--;
            }
                delete job;
        }
    }
}


// Private members go here.
// Protected members go here.
