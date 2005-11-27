#include "webfilefetcher.hpp"

#define WEB_FILEBUFFERSIZE 16000

WebFileFetcher::WebFileFetcher(WebRequest *request, string filename) : WebDataFetcher(request)
{
    //   request->client->send_command("200 file returned");
    FILE *file;
    file = fopen(filename.c_str(), "r");
    if(file) {
        fseek(file, 0, SEEK_SET);
        char buffer[WEB_FILEBUFFERSIZE];
        while (fgets(buffer, WEB_FILEBUFFERSIZE, file)) {
            buffer[strlen(buffer) -1] = '\0';
            output_lines.push_back(buffer);
        } 
        fclose(file);
    } else {
        output_lines.push_back("404 not found");
    }
    num_lines = output_lines.size();
}

WebFileFetcher::~WebFileFetcher()
{
}


