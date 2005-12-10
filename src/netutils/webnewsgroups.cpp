#include "webnewsgroups.hpp"
#include "netcentral.hpp"
#include "console.hpp"
#include <sstream>

using std::stringstream;

WebNewsGroups::WebNewsGroups(WebRequest *request) : WebDataFetcher(request)
{
    output_lines.push_back(info_update_string());
    output_lines.push_back("num||full|num");
    num_lines = newsgroups.size();
    for(Uint32 i=0; i<num_lines; ++i) {
        output_lines.push_back(status(newsgroups[i]));
    }
    num_lines = output_lines.size();
}

WebNewsGroups::~WebNewsGroups()
{
}

string WebNewsGroups::status(NewsGroup *group)
{
    stringstream buf;

    buf << group->name 
        << "||fetch_data('/postsets?name=" << group->name << "')|" << group->postsets.size()
  << "||ping_url('/update_newsgroup?name=" << group->name << "')|" << "Update" 
        << "||fetch_data('/postsets?name=" << group->name << "')|" << group->name
        << "||fetch_data('/postsets?name=" << group->name << "')|" << group->last_article_number - group->first_article_number;

    return buf.str();
}

string WebNewsGroups::info_update_string(void)
{
    stringstream out;
    out << WebDataFetcher::info_update_string()
        << " update_heading('Newsgroups');";
    return out.str();
}

