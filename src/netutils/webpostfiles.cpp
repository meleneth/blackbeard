#include "webpostfiles.hpp"
#include "netcentral.hpp"
#include "console.hpp"
#include "config.hpp"

#include<sstream>
#include<iomanip>

using std::stringstream;
using std::setprecision;

WebPostFiles::WebPostFiles(WebRequest *request) : WebDataFetcher(request)
{
    output_lines.push_back("");
    output_lines.push_back("|full|cmd|num|num");

    NewsGroup *group = group_for_name(request->param("name"));
    set = group->postsets[request->paramn("psi")];

    num_lines = set->files.size();
    for(Uint32 i=0; i<num_lines; ++i) {
        if(set->files[i]->tick > request->paramn("tick")) {
            output_lines.push_back(post_file_line(set->files[i], i));
        }
    }

    output_lines[0] = info_update_string();
    num_lines = output_lines.size();
}

WebPostFiles::~WebPostFiles()
{
}

string WebPostFiles::info_update_string(void)
{
    stringstream s;
    s << "last_data_fetch = \"/postfiles?name=" << request->param("name") << ";tick=" << config->tick << "\"; "
      <<  WebDataFetcher::info_update_string()
      << "update_heading('" + set->subject + "');";

    if(request->paramn("tick"))
        s << "mode=\"update\";";
    return s.str();
}

string WebPostFiles::post_file_line(PostFile *file, Uint32 file_index)
{
    WebRequest r(request->get_uri());
    stringstream file_line;

    stringstream s;

    r.param("pfi", file_index);
    r.filename = "download_file";

    s << table_id(file->filename)
      << "||ping_url(" << r.get_uri() << "')|Download";

    r.filename = "viewfile";
    s << "||view_file('" << r.get_uri() << "')|" << file->filename
      << "|| |" << file->status
      << "|| |" << file->downloaded_pieces << "/"  << file->num_pieces
      << "|| |";

    if(file->num_pieces == file->downloaded_pieces){
        s << "100%";
    }else{
        if(file->num_pieces > 0)
           s << setprecision(3)
             << ((double)file->downloaded_pieces / (double)file->num_pieces) * (double) 100
             << "%";
    }
    return s.str();
}

