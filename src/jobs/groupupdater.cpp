#include "groupupdater.hpp"
#include "nntpserver.hpp"
#include "console.hpp"

GroupUpdater::GroupUpdater(NewsGroup *group)
{
    this->group = group;
}

GroupUpdater::~GroupUpdater()
{
}


void GroupUpdater::process(void)
{
    NNTPServer *server = (NNTPServer *) srv;

    if(sent_command){
        if(server->_status == NS_CONNECTED){
            console->log("Update dem groups");
            console->log("Attempting to update with info " + server->responses[0]);
        }
    }else{
        server->send_command("group " + group->name);
        sent_command = 1;
    }
}

