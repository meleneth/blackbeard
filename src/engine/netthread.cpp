#include "netthread.hpp"
#include "console.hpp"
#include"globals.hpp"

// Public data members go here.
NetThread::NetThread(Config *cfg) // Constructor
{
    this->config = cfg;
    connection = new NNTPServer(config->news_server, config->news_port);
}
    
NetThread::~NetThread() // Destructor
{
}

void NetThread::Execute(void)
{
    console->log("Initialized.");
    console->log("Connecting to " + config->news_server + " to grab article list for group " + config->news_group);

    console->log("Selecting group " + config->news_group);
    connection->group(config->news_group);
    connection->xover(1);

    if(current_postset){
        retrieve(current_postset);
    }
/*
    connection->last();
    connection->help();
    connection->date();
    connection->next();
    connection->stat();

    long art_id = 1;
    connection->xover(1);
    connection->article(art_id);
    connection->head(art_id);
    connection->body(art_id);
*/
    console->log("---- End of pre-configured commands ----");
    while(1){
        connection->read_packets();
        while(connection->has_data_waiting()){
            console->log(connection->get_line());
        }
    }
}

void NetThread::retrieve(PostSet *postset)
{
}

// Private members go here.
// Protected members go here.
