#include "webpostsets.hpp"
#include "netcentral.hpp"
#include "console.hpp"
#include "strutil.hpp"
#include "config.hpp"
#include "webdefines.hpp"
#include "webshared.hpp"

#include<sstream>
#include<iomanip>
using std::stringstream;
using std::setprecision;


WebPostSets::WebPostSets(WebRequest *request) : WebDataFetcher(request)
{
    group = request->newsgroup();
    group->needs_postsets();

    output_lines.push_back("");
    output_lines.push_back("num||num||num|full");
    num_lines = group->postsets.size();
    Uint32 request_tick = request->paramn("tick");
    Uint32 got_num = request->paramn("got_num");
    Uint32 valid_num = 0;
    Uint32 next_tick = config->tick;
    Uint32 last_got_num = 0;

    for(Uint32 i=0; i<num_lines; ++i) {
        PostSet *set = group->postsets[i];
        //if(set->num_files() > 10) {
            valid_num++;
            if(valid_num > got_num) {
                if(valid_num < (got_num + MAX_ITEMS_PER_PAGE)) {
                    if(set->tick > request_tick) {
                        output_lines.push_back(status(set));
                        last_got_num = valid_num;
                    }
                } else {
                    next_tick = request_tick;
                }
            }
        //}
    }

    
    num_lines = output_lines.size();
    stringstream s;

    s << "new Pager(20, " << got_num << ", " << valid_num << ", '/postsets?ngi=" << request->paramn("ngi") << ";got_num=');" << info_update_string();
    output_lines[0] = s.str();
}

WebPostSets::~WebPostSets()
{
}

string WebPostSets::status(PostSet *set)
{

    return postset_status_line(request, set);
}

string WebPostSets::info_update_string(void)
{
    WebRequest r(request->get_uri());
    r.param("tick", config->tick);

    stringstream s;
    s << "ui.enable_search_screen(\"search?ngi=" << request->paramn("ngi") << "\", \"" << group->name <<"\"); " 
      << "ui.last_url = \"" << r.get_uri() << "\"; "
      <<  WebDataFetcher::info_update_string()
      << "ui.update_heading('" << group->name << "');";

    return s.str();
}

