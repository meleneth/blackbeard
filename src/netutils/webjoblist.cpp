#include "webjoblist.hpp"
#include "netcentral.hpp"
#include "console.hpp"

WebJobList::WebJobList(WebRequest *request) : WebDataFetcher(request)
{
    num_jobs = 0;
    job_no = 0;
    num_jobs = netcentral->active_jobs.size();
    for(Uint32 i=0; i<num_jobs; ++i) {
        Job *j = netcentral->active_jobs[i];
        job_list.push_back(j->status_line());
    }
}

WebJobList::~WebJobList()
{
}

int WebJobList::tick(void)
{
    if(num_jobs)
        request->client->send_command(job_list[job_no]);
    return ++job_no < num_jobs;
}

