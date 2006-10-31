#include "webnewsgroups.hpp"
#include "netcentral.hpp"
#include "console.hpp"
#include <sstream>
#include "webrequest.hpp"
#include "config.hpp"

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
    r.delete_param("tick");
    r.param("ngi", index);

    buf << "ng" << index
        << "|| |" << group->num_postsets();

    r.filename = "update_newsgroup";
    buf << "||ping_url('" << r.get_uri() << "')|" << "Update";
    
    r.filename = "postsets";
    buf << "||ui.open_screen_with_paged_url_data('" << r.get_uri() <<  "')|" << group->name
        << "|| |" << group->last_article_number - group->first_article_number;

    return buf.str();
}

string WebNewsGroups::info_update_string(void)
{
    stringstream out;
    WebRequest r(request->get_uri());
    r.param("tick", config->tick);

    out << WebDataFetcher::info_update_string()
        << "ui.last_url=\""<< r.get_uri() << "\""
        << " ui.update_heading('Newsgroups');";
    return out.str();
}

