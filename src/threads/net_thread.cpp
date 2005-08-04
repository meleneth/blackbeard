#include<sys/types.h>
#include<sys/stat.h>
#include"net_thread.hpp"
#include"console.hpp"
#include"newsgrouppost.hpp"
#include"yenc_decoder.hpp"
#include"postsetjob.hpp"
#include"jobqueue.hpp"

// Public data members go here.
NetThread::NetThread() // Constructor
{
    _connection = NULL;
}
    
NetThread::~NetThread() // Destructor
{
}

void NetThread::Execute(void)
{
    console->log("Initialized.");

    console->log("---- End of pre-configured commands ----");
    while(1){
        Job *job = jobqueue->get_next_text_job();
        if(job){
            job->process(get_connection());
            if(job->is_finished){
                jobqueue->finish(job);
            } else {
                jobqueue->add_text_job(job);
            }
        }
        sleep(1);
    }
}

NNTPServer *NetThread::get_connection()
{
    if(_connection){
        if(_connection->connected)
            return _connection;
    }

    _connection = new NNTPServer(config->news_server, config->news_port);
    return _connection;
}

