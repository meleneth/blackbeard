#include "webnewsgroups.hpp"
#include "netcentral.hpp"
#include "console.hpp"
#include <sstream>
#include "webrequest.hpp"

using std::stringstream;

WebNewsGroups::WebNewsGroups(WebRequest *request) : WebDataFetcher(request)
{
    output_lines.push_back(info_update_string());
    output_lines.push_back("num||full|num");
    num_lines = newsgroups.size();
    for(Uint32 i=0; i<num_lines; ++i) {
        output_lines.push_back(status(newsgroups[i], i));
    }
    num_lines = output_lines.size();
}

WebNewsGroups::~WebNewsGroups()
{
}

string WebNewsGroups::status(NewsGroup *group, Uint32 index)
{
    stringstream buf;
    WebRequest r(request->get_uri());
    r.filename = "postsets";
    r.param("ngi", index);

    buf << group->name
        << "||fetch_data('" << r.get_uri() << "')|" << group->postsets.size();

    r.filename = "update_newsgroup";
    buf << "||ping_url('" << r.get_uri() << "')|" << "Update";
    
    r.filename = "postsets";
    buf << "||fetch_data('" << r.get_uri() <<  "')|" << group->name
        << "||fetch_data('" << r.get_uri() <<  "')|" << group->last_article_number - group->first_article_number;

    return buf.str();
}

string WebNewsGroups::info_update_string(void)
{
    stringstream out;
    out << WebDataFetcher::info_update_string()
        << " update_heading('Newsgroups');";
    return out.str();
}

