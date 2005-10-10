#include "groupupdater.hpp"
#include "nntpserver.hpp"
#include "console.hpp"

GroupUpdater::GroupUpdater(NewsGroup *group)
{
    this->group = group;
    sub_job = NULL;
}

GroupUpdater::~GroupUpdater()
{
}


void GroupUpdater::process(void)
{
    NNTPServer *server = (NNTPServer *) srv;
    if(sub_job){
        sub_job->process();
    }else{
        if(sent_command){
            if(server->has_data_waiting()){
            }
        }else{
            server->send_command("group " + group->name);
            sent_command = 1;
        }
        console->log("Update dem groups");
    }
}

