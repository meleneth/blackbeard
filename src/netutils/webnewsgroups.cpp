#include "webnewsgroups.hpp"
#include "netcentral.hpp"
#include "console.hpp"

WebNewsGroups::WebNewsGroups(WebRequest *request) : WebDataFetcher(request)
{
    num_lines = newsgroups.size();
    for(Uint32 i=0; i<num_lines; ++i) {
        output_lines.push_back(newsgroups[i]->name);
    }
}

WebNewsGroups::~WebNewsGroups()
{
}

