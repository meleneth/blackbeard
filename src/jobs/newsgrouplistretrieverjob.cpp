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
}

NewsGroupListRetrieverJob::~NewsGroupListRetrieverJob()
{
}

string NewsGroupListRetrieverJob::status_line(void)
{
    return "Status for NewsGroupListRetrieverJob [" + _status + "]";
}

void NewsGroupListRetrieverJob::process()
{
    _status = "Retrieving group list";
    NNTPServer *server = (NNTPServer *)srv;

    if(!sent_list_command){
        if(server->_status == NS_CONNECTED){
            sent_list_command = 1;
            server->send_command(LIST);
        }
        return;
    }

    string line = server->get_next_multi_line();
    while(server->has_data_waiting()){
        if(server->is_multiline_reading) {
            if(breaker->match(line)){
                NewsGroup *group = group_for_name(breaker->results[0]);
                group->last_article_number = breaker->get_piecen(1);
                group->first_article_number = breaker->get_piecen(2);
            }else{
                group_for_name(line);
            }
            line = server->get_next_multi_line();
        }else{
            is_finished = 1;
        }
    }
}

