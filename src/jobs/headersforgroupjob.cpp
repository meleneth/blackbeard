#include "headersforgroupjob.hpp"
#include"nntpserver.hpp"
#include <fstream>   // file I/O
#include<sstream>

using std::ios;
using std::endl;
using std::stringstream;

HeadersForGroupJob::HeadersForGroupJob(NewsGroup *group)
{
    this->group = group;
    net_cmds.push_back("group " + group->name);
    net_cmds.push_back("xover 1-");
    if(config->dump_subject_file){
        out.open("headers.log", ios::out);
    }
}

HeadersForGroupJob::HeadersForGroupJob(NewsGroup *group, Uint32 start_id, Uint32 stop_id)
{
    this->group = group;
    net_cmds.push_back("group " + group->name);
    stringstream c;
    c << "xover " << start_id << "-";
    if(stop_id)
        c << stop_id;
    net_cmds.push_back(c.str());
    if(config->dump_subject_file){
        out.open("headers.log", ios::out);
    }
}

HeadersForGroupJob::~HeadersForGroupJob()
{
}

string HeadersForGroupJob::status_line(void)
{
    return "default status for HeadersForGroupJob";
}

void HeadersForGroupJob::process_line(string line)
{
    if(config->dump_subject_file){
        out << line << endl;
    }else{
        group->header_scoop(line);
    }
}
