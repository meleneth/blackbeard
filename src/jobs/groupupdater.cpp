#include "groupupdater.hpp"
#include "nntpserver.hpp"
#include "console.hpp"

GroupUpdater::GroupUpdater(NewsGroup *group)
{
    this->group = group;
    sent_command = 0;
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
            update_group(server->responses[0]);
            is_finished = 1;
        }
    }else{
        if(server->_status == NS_CONNECTED){
            server->send_command("group " + group->name);
            sent_command = 1;
        }
    }
}

void GroupUpdater::update_group(string line)
{
    console->log("Attempting to update with info " + line);
    StringPattern match(6);
    match.add_breaker(" ");
    group->expire_old_postsets(match.get_piecen(0));
}
