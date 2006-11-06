#include "websearch.hpp"
#include "webdefines.hpp"
#include "webshared.hpp"
#include "config.hpp"

#include <sstream>

using std::stringstream;

WebSearch::WebSearch(WebRequest *request) : WebDataFetcher(request)
{
    output_lines.push_back(info_update_string());
    output_lines.push_back("num||num||num|full");

    Uint32 request_tick = request->paramn("tick");
    Uint32 got_num = request->paramn("got_num");

    vector<PostSet *> results;
    request->newsgroup()->search(results, request->param("q"));

    Uint32 num_results = results.size();
    Uint32 end_window;

    if(num_results < MAX_ITEMS_PER_PAGE) {
        got_num = 0;
        end_window = num_results;
    }else {
        if(got_num > (num_results - MAX_ITEMS_PER_PAGE)) {
            got_num = num_results - MAX_ITEMS_PER_PAGE;
            end_window = num_results;
        } else {
            end_window = got_num + MAX_ITEMS_PER_PAGE;
        }
    }

    //for(Uint32 i=got_num; i<end_window; ++i) {
    for(Uint32 i=0; i<num_results; ++i) {
        if(results[i]->tick > request_tick) {
            output_lines.push_back(postset_status_line(request, results[i]));
        }
    }
    num_lines = output_lines.size();
}

WebSearch::~WebSearch()
{
}

string WebSearch::info_update_string(void)
{
    stringstream out;
    WebRequest r(request->get_uri());
    r.param("tick", config->tick);

    out << WebDataFetcher::info_update_string()
        << "ui.last_url=\""<< r.get_uri() << "\""
        << " ui.update_heading('Newsgroups');";
    return out.str();
}

