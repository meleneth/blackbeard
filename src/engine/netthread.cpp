#include "netthread.hpp"
#include "console.hpp"

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

    console->log("Logging into server");
    connection->login("arnuga", "leper56");

    console->log("Selecting group " + config->news_group);
    connection->group(config->news_group);
    connection->xover();
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

    while(1){
        connection->read_packets();
        connection->slice_buffer_strings();
        while(connection->has_data_waiting()){
            console->log(connection->get_line());
        }

    }
}

// Private members go here.
// Protected members go here.
