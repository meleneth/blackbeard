#include "webpostsets.hpp"
#include "netcentral.hpp"
#include "console.hpp"
#include "strutil.hpp"
#include "config.hpp"

#include<sstream>
#include<iomanip>
using std::stringstream;
using std::setprecision;

#define MAX_PER_REQUEST 100

WebPostSets::WebPostSets(WebRequest *request) : WebDataFetcher(request)
{
    group = request->newsgroup();
    group->needs_postsets();

    output_lines.push_back("");
    output_lines.push_back("num|num||num|full");
    num_lines = group->postsets.size();
    Uint32 request_tick = request->paramn("tick");
    Uint32 got_num = request->paramn("got_num");
    Uint32 valid_num = 0;
    Uint32 next_tick = config->tick;
    Uint32 last_got_num = 0;

    for(Uint32 i=0; i<num_lines; ++i) {
        PostSet *set = group->postsets[i];
        if(set->tick > request_tick) {
            if(set->num_files() > 10) {
                valid_num++;
                if(valid_num > got_num) {
                    if(valid_num < (got_num + MAX_PER_REQUEST)) {
                        output_lines.push_back(status(set, i));
                        last_got_num = valid_num;
                    } else {
                        next_tick = request_tick;
                        i = num_lines;
                    }
                }
            }
        }
    }
    num_lines = output_lines.size();
    stringstream s;
    s << "tab.last_retrieve = '/postsets?ngi=" << request->paramn("ngi") << ";tick=" << next_tick << ";got_num=" << last_got_num << ";';" << info_update_string();
    output_lines[0] = s.str();
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
    s   << "||ui.open_tab_with_url_data('postset details', '"<< r.get_uri() << "')|" << js_escape( replace_substrings(set->subject, "|", "").substr(0, 80));

    return s.str();
}

string WebPostSets::info_update_string(void)
{
    WebRequest r(request->get_uri());
    r.param("tick", config->tick);
    stringstream s;
    s <<  WebDataFetcher::info_update_string()
      << "update_heading('" << group->name << "');";

    return s.str();
}

