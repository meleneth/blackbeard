#include "webnewsgroups.hpp"
#include "netcentral.hpp"
#include "console.hpp"

WebNewsGroups::WebNewsGroups(WebRequest *request) : WebDataFetcher(request)
{
    output_lines.push_back("fetch_data('/postsets/%s')");
    num_lines = newsgroups.size();
    for(Uint32 i=0; i<num_lines; ++i) {
        output_lines.push_back(newsgroups[i]->name+ "|" + newsgroups[i]->name);
    }
    num_lines = output_lines.size();
}

WebNewsGroups::~WebNewsGroups()
{
}

