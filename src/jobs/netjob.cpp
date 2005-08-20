#include "netjob.hpp"
#include"console.hpp"
#include"nntpserver.hpp"


NetJob::NetJob()
{
    net_cmd = "";
    sent_command = 0;
}

NetJob::~NetJob()
{
}

void NetJob::process(void)
{
    NNTPServer *server = (NNTPServer *) srv;
    Uint32 max_lines = MAX_NETJOB_LINES_PER_SLICE;

    if(!sent_command){
        if(server->_status == NS_CONNECTED){
            sent_command = 1;
            server->send_command(net_cmd);
            server->is_multiline_reading = 1;
        }
        return;
    }

    while(max_lines-- && server->has_data_waiting()){
        string line = server->get_next_multi_line();
        if(server->is_multiline_reading) {
            process_line(line);
        }
    }
    return;
}

void NetJob::process_line(string line)
{
    console->log("Probably want to override NetJob::process_line");
}
