#include "webpostsets.hpp"
#include "netcentral.hpp"
#include "console.hpp"
#include "strutil.hpp"
#include "config.hpp"

#include<sstream>
#include<iomanip>
using std::stringstream;
using std::setprecision;

WebPostSets::WebPostSets(WebRequest *request) : WebDataFetcher(request)
{
    group = request->newsgroup();
    group->needs_postsets();

    output_lines.push_back(info_update_string());
    output_lines.push_back("num||full|num");
    num_lines = group->postsets.size();

    for(Uint32 i=0; i<num_lines; ++i) {
        if(group->postsets[i]->tick > request->paramn("tick")) {
            if(group->postsets[i]->num_files() > 10) {
                output_lines.push_back(status(group->postsets[i], i));
            }
        }
    }
    num_lines = output_lines.size();
}

WebPostSets::~WebPostSets()
{
}

string WebPostSets::status(PostSet *set, Uint32 index)
{
    WebRequest r = WebRequest(request->get_uri());

    r.delete_param("tick");
    r.param("psi", index);
    r.filename = "postfiles";

    stringstream s;
    s   << index
        << "|| |" << set->num_files()
        << "|| |" << human_readable_bytes(set->num_bytes());

    r.filename = "downloadpostset";
    s   << "|| ping_url('" << r.get_uri() << "')| Download"
        << "|| |" << setprecision(3) << set->completed_percent() << "%";

    r.filename = "postfiles";
    s   << "||fetch_data('" << r.get_uri() << "')|" << js_escape( replace_substrings(set->subject, "|", "").substr(0, 80));

    return s.str();
}

string WebPostSets::info_update_string(void)
{
    WebRequest r(request->get_uri());
    r.param("tick", config->tick);
    stringstream s;
    s << "last_data_fetch = \"" << r.get_uri() << "\"; "
      <<  WebDataFetcher::info_update_string()
      << "update_heading('" << group->name << "');";

    if(request->paramn("tick"))
        s << "mode=\"update\";";
    return s.str();
}

