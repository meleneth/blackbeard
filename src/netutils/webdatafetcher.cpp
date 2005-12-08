#include "webdatafetcher.hpp"
#include "netcentral.hpp"
#include "config.hpp"

#include <sstream>

using std::stringstream;

WebDataFetcher::WebDataFetcher(WebRequest *r)
{
    this->request = r;
    line_no = 0;
    num_lines = 0;
}

WebDataFetcher::~WebDataFetcher()
{
    delete request;
}

string WebDataFetcher::info_update_string(void)
{
    stringstream update_cmd;
    update_cmd << "last_data_fetch = "
               << config->tick 
               << "; update_meters('(" 
               << netcentral->jobs.size() 
               << "/" 
               << netcentral->active_jobs.size()
               << ")', '" 
               << netcentral->krate()
               << "');";
    return update_cmd.str();
}

int WebDataFetcher::tick(void)
{
    if(num_lines)
        request->client->send_command(output_lines[line_no]);
    return ++line_no < num_lines;
}

