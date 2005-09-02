#include "newsgrouplistretrieverjob.hpp"
#include"nntpserver.hpp"

#include<string>

using std::string;

NewsGroupListRetrieverJob::NewsGroupListRetrieverJob()
{
    breaker = new StringPattern(4);
    breaker->add_breaker(0);
    breaker->add_breaker(" ");
    breaker->add_breaker(1);
    breaker->add_breaker(" ");
    breaker->add_breaker(2);
    breaker->add_breaker(" ");
    breaker->add_breaker(3);
    sent_list_command = 0;
    _status = "Waiting for CPU";
    net_cmds.push_back(LIST);
}

NewsGroupListRetrieverJob::~NewsGroupListRetrieverJob()
{
}

string NewsGroupListRetrieverJob::status_line(void)
{
    return "Status for NewsGroupListRetrieverJob [" + _status + "]";
}

void NewsGroupListRetrieverJob::process_line(string line)
{
    _status = "Retrieving group list";
    if(breaker->match(line)){
        NewsGroup *group = new NewsGroup(breaker->results[0]);
        newsgroups.push_back(group);
        group->last_article_number = breaker->get_piecen(1);
        group->first_article_number = breaker->get_piecen(2);
    }else{
        group_for_name(line);
    }
}

