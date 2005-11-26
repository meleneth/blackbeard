#include "webpostfiles.hpp"
#include "netcentral.hpp"
#include "console.hpp"

#include<sstream>

using std::stringstream;

WebPostFiles::WebPostFiles(WebRequest *request) : WebDataFetcher(request)
{
    output_lines.push_back("return false");
    StringPattern splitter = StringPattern(2);
    splitter.add_breaker(0);
    splitter.add_breaker(",");
    splitter.add_breaker(1);

    if(splitter.match(request->filename)) {
        NewsGroup *group = group_for_name(splitter.results[0]);
        PostSet *p = group->postsets[splitter.get_piecen(1)];
        
        num_lines = p->files.size();
        for(Uint32 i=0; i<num_lines; ++i) {
            output_lines.push_back("|" + p->files[i]->status_string());
        }
    }
    num_lines = output_lines.size();
}

WebPostFiles::~WebPostFiles()
{
}

