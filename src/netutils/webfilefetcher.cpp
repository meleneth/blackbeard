#include "webfilefetcher.hpp"

WebFileFetcher::WebFileFetcher(WebRequest *request, string filename) : WebDataFetcher(request)
{

//   request->client->send_command("200 file returned");

    file = fopen(filename.c_str(), "r");
    fseek(file, 0, SEEK_SET);
}

WebFileFetcher::~WebFileFetcher()
{
    fclose(file);
}

int WebFileFetcher::tick()
{
    if (fgets(buffer, 4096 ,file)) {
        buffer[strlen(buffer) -1] = '\0';
        request->client->send_command(buffer);
    } else {
        return 0;
    }
    return 1;
}

