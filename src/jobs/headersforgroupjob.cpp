#include "headersforgroupjob.hpp"
#include"nntpserver.hpp"

HeadersForGroupJob::HeadersForGroupJob(NewsGroup *group)
{
    this->group = group;
    net_cmds.push_back("group " + group->name);
    net_cmds.push_back("xover 1-");
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
    group->header_scoop(line);
}
