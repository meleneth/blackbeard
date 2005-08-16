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
}

NewsGroupListRetrieverJob::~NewsGroupListRetrieverJob()
{
}

string NewsGroupListRetrieverJob::status_line(void)
{
    return "Status for NGLRJ [" + _status + "]";
}

void NewsGroupListRetrieverJob::process(void *ptr)
{
    _status = "Retrieving group list";
    NNTPServer *srv = (NNTPServer *)ptr;

    if(!sent_list_command){
        sent_list_command = 1;
        srv->send_command(LIST);
    }

    string line = srv->get_next_multi_line();
    while(srv->is_multiline_reading) {
        line = srv->get_next_multi_line();
    }

    if(breaker->match(line)){
        NewsGroup *group = group_for_name(breaker->results[0]);
        group->last_article_number = breaker->get_piecen(1);
        group->first_article_number = breaker->get_piecen(2);
    }else{
        group_for_name(line);
    }
    
}

