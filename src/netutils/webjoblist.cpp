#include "webjoblist.hpp"
#include "netcentral.hpp"
#include "console.hpp"

WebJobList::WebJobList(WebRequest *request) : WebDataFetcher(request)
{
    output_lines.push_back("return false");
    num_lines = netcentral->active_jobs.size();
    for(Uint32 i=0; i<num_lines; ++i) {
        Job *j = netcentral->active_jobs[i];
        output_lines.push_back("|" + j->status_line());
    }
    num_lines = output_lines.size();
}

WebJobList::~WebJobList()
{
}

