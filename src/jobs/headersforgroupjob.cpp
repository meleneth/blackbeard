#include "headersforgroupjob.hpp"
#include"nntpserver.hpp"
#include <fstream>   // file I/O
#include<sstream>

using std::ios;
using std::endl;

HeadersForGroupJob::HeadersForGroupJob(NewsGroup *group)
{
    this->group = group;
    net_cmds.push_back("group " + group->name);
    net_cmds.push_back("xover 1-");
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
