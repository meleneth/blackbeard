#include "netjob.hpp"
#include"console.hpp"
#include"nntpserver.hpp"

#define NOT_FOUND_ERROR "423"

NetJob::NetJob()
{
    is_finished = 0;
    group = NULL;
}

NetJob::~NetJob()
{
}

void NetJob::process(void)
{
    NNTPServer *server = (NNTPServer *) srv;
    Uint32 max_lines = MAX_NETJOB_LINES_PER_SLICE;

    if(net_cmds.begin() != net_cmds.end()){
        if(server){
            if(server->_status == NS_CONNECTED){
                if(group){
                    if(group != server->newsgroup){
                        string cmd = "group " + group->name;
                        server->send_command(cmd);
                        server->newsgroup = group;
                        return;
                    }
                }else{
                    server->newsgroup = NULL;
                }
                list<string>::iterator i = net_cmds.begin();
                string net_cmd = *i;
                net_cmds.erase(i);
                server->send_command(net_cmd);
                if(!net_cmds.size())
                    server->is_multiline_reading = 1;
            }
        }else{
            console->log("No server for net job??");
        }
        return;
    }

    while(max_lines-- && server->has_data_waiting()){
        string line = server->get_next_multi_line();
        if(server->is_multiline_reading) {
            process_line(line);
        }else{
            finish();
        }
    }
    return;
}

void NetJob::process_line(string line)
{
    console->log("Probably want to override NetJob::process_line");
}

void NetJob::finish(void)
{
    is_finished = 1;
    console->log("Job finished");
}
