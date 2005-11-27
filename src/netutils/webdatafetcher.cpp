#include "webdatafetcher.hpp"
#include "netcentral.hpp"

#include <sstream>

using std::stringstream;

WebDataFetcher::WebDataFetcher(WebRequest *r)
{
    this->request = r;
    line_no = 0;
    num_lines = 0;
    stringstream update_cmd;

    update_cmd << "update_meters('(" 
               << netcentral->jobs.size() 
               << "/" 
               << netcentral->active_jobs.size()
               << ")', '" 
               << netcentral->krate()
               << "');";

    output_lines.push_back(update_cmd.str());
}

WebDataFetcher::~WebDataFetcher()
{
    delete request;
}

int WebDataFetcher::tick(void)
{
    if(num_lines)
        request->client->send_command(output_lines[line_no]);
    return ++line_no < num_lines;
}

