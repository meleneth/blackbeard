#include "webpostfiles.hpp"
#include "netcentral.hpp"
#include "console.hpp"
#include "config.hpp"
#include "strutil.hpp"

#include <sstream>
#include <iomanip>

using std::stringstream;
using std::setprecision;

WebPostFiles::WebPostFiles(WebRequest *request) : WebDataFetcher(request)
{
    output_lines.push_back("");
    output_lines.push_back("|full|cmd|num|num");

    set = request->postset();

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
    WebRequest r(request->get_uri());

    r.param("tick", config->tick);

    stringstream s;
    s << "last_data_fetch = \"" << r.get_uri() << "\"; "
      <<  WebDataFetcher::info_update_string()
      << "update_heading('" + js_escape(set->subject) + "');";

    if(request->paramn("tick"))
        s << "mode=\"update\";";
    return s.str();
}

string WebPostFiles::post_file_line(PostFile *file, Uint32 file_index)
{
    WebRequest r(request->get_uri());
    stringstream file_line;

    stringstream s;
    r.delete_param("tick");
    r.param("pfi", file_index);
    r.filename = "downloadpostfile";

    s << table_id(file->filename)
      << "||ping_url('" << r.get_uri() << "')|Download"
      << "||";
    
    if(file_is_viewable(file->filename)){
        if(0 == file->status.compare("Finished")) {
            r.filename = "viewfile";
            s << "view_file('" << r.get_uri() << "')";
        }
    }
    r.filename = "change_file_status";
    s << " |" << replace_substrings(file->filename, "|", "").substr(0, 80)
      << "||ping_url('"<< r.get_uri() <<"')|" << file->status
      << "|| |" << file->num_downloaded_pieces() << "/"  << file->pieces.size()
      << "|| |";

    if(file->pieces.size() == file->num_downloaded_pieces()){
        s << "100%";
    }else{
        if(file->pieces.size() > 0)
           s << setprecision(3)
             << ((double)file->num_downloaded_pieces() / (double)file->pieces.size()) * (double) 100
             << "%";
    }
    return s.str();
}

Uint32 WebPostFiles::file_is_viewable(string filename)
{
    Uint32 len = filename.length();
    if(len > 4){
        string extension = filename.substr(len-4, 4);
        if(0 == extension.compare(".nfo"))
            return 1;
        if(0 == extension.compare(".jpg"))
            return 1;
        if(0 == extension.compare(".gif"))
            return 1;
        if(0 == extension.compare(".png"))
            return 1;
    }
    return 0;
}

