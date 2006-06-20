#include "webpostsets.hpp"
#include "netcentral.hpp"
#include "console.hpp"
#include "strutil.hpp"
#include "config.hpp"

#include<sstream>
#include<iomanip>
using std::stringstream;
using std::setprecision;

#define MAX_PER_REQUEST 20

WebPostSets::WebPostSets(WebRequest *request) : WebDataFetcher(request)
{
    group = request->newsgroup();
    group->needs_postsets();

    output_lines.push_back("");
    output_lines.push_back("num||full|num");
    num_lines = group->postsets.size();
    Uint32 request_tick = request->paramn("tick");
    Uint32 got_num = request->paramn("got_num");
    Uint32 valid_num = 0;

    for(Uint32 i=0; i<num_lines; ++i) {
        PostSet *set = group->postsets[i];
        if(set->tick > request_tick) {
            if(set->num_files() > 10) {
                valid_num++;
                if(valid_num > got_num) {
                    if(valid_num < (got_num + MAX_PER_REQUEST)) {
                        output_lines.push_back(status(set, i));
                    }
                }
            }
        }
    }
    num_lines = output_lines.size();
    output_lines[0] = info_update_string();
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
    s   << "ps_" << set->group->index() << "_" << index
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

