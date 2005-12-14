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

    output_lines.push_back(info_update_string());
    output_lines.push_back("num||full|num");
    num_lines = group->postsets.size();

    for(Uint32 i=0; i<num_lines; ++i) {
        if(group->postsets[i]->tick > request->paramn("tick")) {
            output_lines.push_back(status(group->postsets[i], i));
        }
    }
    num_lines = output_lines.size();
}

WebPostSets::~WebPostSets()
{
}

string WebPostSets::status(PostSet *set, Uint32 index)
{
    WebRequest r = WebRequest(request->get_uri() + ";psi=bar");

    r.delete_param("tick");
    r.param("psi", index);
    r.filename = "postfiles";

    stringstream s;
    s   << index
        << "||fetch_data('" << r.get_uri() << "')|" << set->num_files << "/" << set->_max_num_files;

    if(1 == 1){
        r.filename = "updatepostset";
        s << "||ping_url('" << r.get_uri() << "');|" << "Update";
    }
    r.filename = "postfiles";
    s   << "||fetch_data('" << r.get_uri() << "')|" << js_escape(set->subject)
        << "||fetch_data('" << r.get_uri() << "')|" << setprecision(3) << set->completed_percent() << "%";
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

