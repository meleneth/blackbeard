#include "groupupdater.hpp"
#include "nntpserver.hpp"
#include "netcentral.hpp"
#include "console.hpp"
#include "headersforgroupjob.hpp"

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
    StringPattern match(5);
    match.add_breaker(0); match.add_breaker("211 ");
    match.add_breaker(1); match.add_breaker(" ");
    match.add_breaker(2); match.add_breaker(" ");
    match.add_breaker(3); match.add_breaker(" ");

    if(match.match(line)){
        group->first_article_number = match.get_piecen(2);
        int highest_article_no = group->highest_seen_article_no();
        high_priority_jobs->add_job(new HeadersForGroupJob(group, highest_article_no, highest_article_no + 1000000));
    }
}
