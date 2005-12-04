#include "webpostsets.hpp"
#include "netcentral.hpp"
#include "console.hpp"
#include "strutil.hpp"

#include<sstream>
#include<iomanip>
using std::stringstream;
using std::setprecision;

WebPostSets::WebPostSets(WebRequest *request) : WebDataFetcher(request)
{
    group = group_for_name(request->filename);
    output_lines.push_back(info_update_string());
    output_lines.push_back("%s");

    output_lines.push_back("num|||num");
    num_lines = group->postsets.size();
    for(Uint32 i=0; i<num_lines; ++i) {
        output_lines.push_back(status(group->postsets[i], i));
    }
    num_lines = output_lines.size();
}

WebPostSets::~WebPostSets()
{
}

string WebPostSets::status(PostSet *set, Uint32 index)
{
    stringstream s;
    s   << "fetch_data('/postfiles/" << set->group->name << "," << index << "')|"
            << set->num_files << "/" << set->_max_num_files;
    if(1 == 1){
        s << "||ping_url('/updatepostset/" << set->group->name << "," << index << "');|"
            << "Update";
    }
    s   << "||fetch_data('/postfiles/" << set->group->name << "," << index << "')|"
            << js_escape(set->subject) 
        << "||fetch_data('/postfiles/" << set->group->name << "," << index << "')|"
            << setprecision(3) << set->completed_percent() << "%";
    return s.str();
}

string WebPostSets::info_update_string(void)
{
    string meters = WebDataFetcher::info_update_string();
    return meters + " update_heading('" + group->name + "');";
}

