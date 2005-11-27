#include "webpostsets.hpp"
#include "netcentral.hpp"
#include "console.hpp"

#include<sstream>

using std::stringstream;

WebPostSets::WebPostSets(WebRequest *request) : WebDataFetcher(request)
{
    output_lines.push_back(info_update_string());
    NewsGroup *group = group_for_name(request->filename);
    output_lines.push_back("fetch_data('/postfiles/%s')");
    
    num_lines = group->postsets.size();
    for(Uint32 i=0; i<num_lines; ++i) {
        stringstream l;
        l << group->name << "," << i << "|" << (group->postsets[i])->status();
        output_lines.push_back(l.str());
    }
    num_lines = output_lines.size();
}

WebPostSets::~WebPostSets()
{
}

