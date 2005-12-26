#include "webfilefetcher.hpp"
#include "file_handle.hpp"

#define WEB_FILEBUFFERSIZE 16000

WebFileFetcher::WebFileFetcher(WebRequest *request, string filename) : WebDataFetcher(request)
{
    //   request->client->send_command("200 file returned");
    FileHandle file(filename);
    size = file.size;
    buf = file.read_whole_file();
    file.close();
}

WebFileFetcher::~WebFileFetcher()
{
}

int WebFileFetcher::tick()
{
    request->client->send_command("Content-type: " + request->content_type);
    request->client->send_data(buf, size);
    return 0;
}
