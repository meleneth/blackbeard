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
        WebRequest r("/postfiles");
        r.filename = "postfiles";
        r.param("ngi", set->group->index());
        r.param("psi", set->index);

        s << i << "|| javascript:ui.open_screen_with_url_data('" << r.get_uri() << "')|"  << file->filename;
    } else {
        s << i << "|| return false" << "| " << job->status_line();
    }
    i++;
    return s.str();
}

