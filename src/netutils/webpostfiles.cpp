#include "webpostfiles.hpp"
#include "netcentral.hpp"
#include "console.hpp"

#include<sstream>
#include<iomanip>

using std::stringstream;
using std::setprecision;

WebPostFiles::WebPostFiles(WebRequest *request) : WebDataFetcher(request)
{
    output_lines.push_back("|full|cmd|num|num");

    StringPattern splitter(2);
    splitter.add_breaker(0);
    splitter.add_breaker(",");
    splitter.add_breaker(1);

    if(splitter.match(request->filename)) {
        NewsGroup *group = group_for_name(splitter.results[0]);
        set = group->postsets[splitter.get_piecen(1)];

        num_lines = set->files.size();
        for(Uint32 i=0; i<num_lines; ++i) {
            output_lines.push_back(post_file_line(set->files[i]));
        }
        output_lines[0] = info_update_string();
    }
    num_lines = output_lines.size();
}

WebPostFiles::~WebPostFiles()
{
}

string WebPostFiles::info_update_string(void)
{
    string meters = WebDataFetcher::info_update_string();
    return meters + " update_heading('" + set->subject + "');";
}

string WebPostFiles::post_file_line(PostFile *file)
{
    stringstream file_line;
    file_line << file->post_set->group->postset_index(file->post_set)
              << "," << file->post_set->file_index(file);

    stringstream s;
    s << file->filename 
    << "||ping_url('/download_file/" << file->post_set->group->name
    << "," << file_line.str() <<"')|Download"
    << "|| |" << file->filename
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

