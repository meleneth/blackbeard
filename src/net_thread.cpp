#include<sys/types.h>
#include<sys/stat.h>
#include"net_thread.hpp"
#include"console.hpp"
#include"globals.hpp"
#include"newsgrouppost.hpp"
#include"yenc_decoder.hpp"

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
        if(_fetch && current_postset){
            retrieve(current_postset);
            _fetch = 0;
        }
        while(connection->has_data_waiting()){
            console->log(connection->get_line());
        }
    }
}

void NetThread::retrieve(PostSet *postset)
{
    struct stat my_stats;
    vector< PostFile * >::iterator v;
    vector< string >::iterator s;

    console->log("Retrieving PostSet");

    string dest_dir = config->blackbeard_dir + "/" + postset->subject;
    console->log(dest_dir);
    if(stat(dest_dir.c_str(), &my_stats) == -1){
        mkdir(dest_dir.c_str(), 01777);
    }
    for (v=postset->files.begin(); v!=postset->files.end(); ++v){
        if(*v)
            (*v)->status = "Marked for retrieval";
    }
    for (v=postset->files.begin(); v!=postset->files.end(); ++v){
        if(*v){
            current_postfile = *v;
            console->log("Retrieveing " + (*v)->filename);
            (*v)->downloaded_pieces = 0;
            (*v)->status = "Downloading";
            for (s=(*v)->pieces.begin(); s!=(*v)->pieces.end(); ++s){
                if((*s).compare("")){
                    NewsGroupPost *newsgrouppost = connection->body(*s);
                    jobqueue->add_decoder_job((*v)->get_decoder(newsgrouppost, dest_dir, (*s)));
                    (*v)->downloaded_pieces++;
                }
            }
            (*v)->status = "Finished";
        }
    }
}

void NetThread::set_retrieve(void)
{
    _fetch = 1;
}
