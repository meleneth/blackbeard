#include "webjoblist.hpp"
#include "netcentral.hpp"
#include "console.hpp"
#include "postfilejob.hpp"
#include "post_set.hpp"
#include <sstream>
#include <string>

using std::string;
using std::stringstream;

WebJobList::WebJobList(WebRequest *request) : WebDataFetcher(request)
{
    output_lines.push_back(info_update_string());
    output_lines.push_back("full");
    num_lines = netcentral->active_jobs.size();

    for(Uint32 i=0; i<num_lines; ++i) {
        output_lines.push_back(line_for_job(netcentral->active_jobs[i]));
    }

    num_lines = output_lines.size();
}

WebJobList::~WebJobList()
{
}

string WebJobList::line_for_job(Job *job)
{
    stringstream s;
    static Uint32 i=0;

    if(job->job_type == POSTFILE_DOWNLOAD) {
        PostfileJob *j = (PostfileJob *)job;
        PostFile *file = j->postfile;
        PostSet *set = file->post_set;

        s << i << "|| fetch_data('/postfiles?name=" << set->group->name << ";index=" << set->group->postset_index(set) << "')";
    } else {
        s << i << "|| return false" << "| " << job->status_line();
    }
    i++;
    return s.str();
}

