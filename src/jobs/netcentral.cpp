#include "netcentral.hpp"
#include "console.hpp"
#include "config.hpp"
#include <iostream>  // I/O 
#include <fstream>   // file I/O
#include <sstream>
#include "nntpserver.hpp"
#include "postsetjob.hpp"
#include "webserver.hpp"

using std::string;
using std::stringstream;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::endl;

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

Uint32 NetCentral::krate(void)
{
    Uint32 max_no = active_jobs.size();
    Uint32 krate = 0;
    Uint32 i;

    for(i = 0; i<max_no; ++i){
        Job *job = active_jobs[i];
        NNTPServer *srv = (NNTPServer *) job->srv;
        krate += srv->krate;
    }
    return krate;
}

void NetCentral::process_jobs(void)
{
/*    if((0 == jobs.size()) && (0 == active_jobs.size())){
#ifdef __WIN32__
	Sleep(10);
#else
        usleep(10);
#endif
        return;
    }
    */

    FD_ZERO(&read_fds);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 10;
    fdmax = 0;

    Uint32 max_jobid = active_jobs.size();
    Uint32 i;
    list<TCPConnection *>::iterator c;

    for(i = 0; i<max_jobid; ++i){
        NNTPServer *connection = (NNTPServer *) active_jobs[i]->srv;
        FD_SET(connection->sockfd, &read_fds);
        if(fdmax < connection->sockfd){
            fdmax = connection->sockfd;
        }
    }

    FD_SET(webserver->listener->sockfd, &read_fds);
    if(webserver->listener->sockfd > fdmax){
        fdmax = webserver->listener->sockfd;
    }

    for(c = webserver->connections.begin(); c != webserver->connections.end(); ++c){
        int fd = (*c)->sockfd;
        FD_SET(fd, &read_fds);
        if(fdmax < fd){
            fdmax = fd;
        }
    }

    int result;
    do { result = select(fdmax+1, &read_fds, NULL, NULL, &tv); } 
    while((result == -1) && (errno == EINTR)); 
    if(result == -1){
        perror("select");
        exit(1);
    }

    if(has_more_jobs()){
        if(active_jobs.size() < config->max_net_connections){
            Job *new_job = get_next_job();
            if(new_job){
                new_job->srv = new NNTPServer(config->news_server, config->news_port);
                active_jobs.push_back(new_job);
                max_jobid = active_jobs.size();
            }
        }    
    }
    
    vector<Job *> still_running;
    for(i = 0; i<max_jobid; ++i){
        Job *job = active_jobs[i];
        NNTPServer *connection = (NNTPServer *) job->srv;

        if(FD_ISSET(connection->sockfd, &read_fds)){
            connection->read_packets();
        }

        connection->tick();
        job->process();

        if(job->is_finished){
            finish_job(job);
            Job *new_job = get_next_job();
            if(new_job){
                new_job->srv = job->srv;
                active_jobs[i] = new_job;
                still_running.push_back(new_job);
            }else{
                delete connection;
                connection = NULL;
            }
            delete job;
        } else {
            still_running.push_back(job);
        }
    }

    for(c = webserver->connections.begin(); c != webserver->connections.end(); ++c){
        if(FD_ISSET((*c)->sockfd, &read_fds)){
            (*c)->read_packets();
        }
    }

    if(FD_ISSET(webserver->listener->sockfd, &read_fds)){
        webserver->handle_new_connection();
    }

    active_jobs = still_running;
}

void NetCentral::save_active_list_file(void)
{
    string filename = config->net_jobs_filename();
    ofstream out;

    out.open(filename.c_str(), ios::out);

    if(out.is_open()){
        Uint32 max_no = job_filenames.size();
        for(Uint32 i=0; i<max_no; i++)
        {
            out << job_filenames[i] << endl;
        }
        out.close();
    }
}

void NetCentral::restore_saved_jobs(void)
{
    ifstream in;
    char linebuffer[1024];
    
    in.open(config->net_jobs_filename().c_str());
    if(in.is_open()){
        in.getline(linebuffer, 1024);

        while(!in.eof()){
            if(strlen(linebuffer)){
                string msg = "Restoring ";
                console->log(msg + linebuffer);
                jobs.push_back(new PostsetJob(linebuffer));
            }
            in.getline(linebuffer, 1024);
        }
    }
}

void NetCentral::add_job(Job *job)
{
    if(job->job_status_filename.compare("")){
        job_filenames.push_back(job->job_status_filename);
        save_active_list_file();
    }
    JobQueue::add_job(job);
}

void NetCentral::finish_job(Job *job)
{
    string filename = job->job_status_filename;
    if(filename.compare("")){
        vector<string> new_filename_list;
        Uint32 max_no = job_filenames.size();
        for(Uint32 i = 0; i<max_no ; ++i)
        {
            if(0 != job_filenames[i].compare(filename)){
                new_filename_list.push_back(filename);
            }
        }
        job_filenames = new_filename_list;
    }
    save_active_list_file();
    JobQueue::finish_job(job);
}

Job *NetCentral::get_next_job(void)
{
    if(high_priority_jobs != this) {
        if(high_priority_jobs->has_more_jobs()){
            return high_priority_jobs->get_next_job();
        }
    }
    return JobQueue::get_next_job();
}

Uint32 NetCentral::has_more_jobs(void)
{
    return this == high_priority_jobs
        ? jobs.size()
        : jobs.size() + high_priority_jobs->has_more_jobs();
}


// Private members go here.
// Protected members go here.
