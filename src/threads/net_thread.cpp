#include<sys/types.h>
#include<sys/stat.h>
#include"net_thread.hpp"
#include"console.hpp"
#include"newsgrouppost.hpp"
#include"yenc_decoder.hpp"
#include"postsetjob.hpp"
#include"jobqueue.hpp"

// Public data members go here.
NetThread::NetThread(Config *cfg) // Constructor
{
    this->config = cfg;
    connection = NULL;
}
    
NetThread::~NetThread() // Destructor
{
}

void NetThread::Execute(void)
{
    console->log("Initialized.");
    if(0 != config->load_file.compare("")){
        console->log("Loading subjects from " + config->load_file);
        NewsGroup *newsgroup = group_for_name("alt.mama");
        newsgroups.push_back(newsgroup);
        newsgroup->load_from_file(config->load_file);
        console->log("All subjects loaded");
    }else if(0 != config->load_group.compare("")){
        console->log("Loading groups from " + config->load_group);
        load_groups_from(config->load_group);
    }else{

        console->log("Connecting to " + config->news_server + " to grab article list for group " + config->news_group);
        connection = new NNTPServer(config->news_server, config->news_port);
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
            /* Downloading broken for now due to refactoring.  Need job queue FIXME */
            /*  if(_fetch && console->current_postset){
                retrieve(console->current_postset);
                _fetch = 0;
            }
            while(connection->has_data_waiting()){
                console->log(connection->get_line());
            } */
        }
    }
}

void NetThread::retrieve(PostSet *postset)
{
    vector< PostFile * >::iterator v;
    vector< string >::iterator s;
    string dest_dir = config->blackbeard_dir + "/" + postset->subject;

    console->log("Retrieving PostSet");
    console->log("Commented out till we have job stuff");
    PostsetJob* postsetjob = new PostsetJob(postset);
    jobqueue->add_text_job(postsetjob);
    console->log("Retrieveing a postset");
}

void NetThread::set_retrieve(void)
{
    _fetch = 1;
}
