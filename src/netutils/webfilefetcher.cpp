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
    Uint32 len = filename.length();

    if(0 == filename.substr(len - 4, 4).compare(".png")){
        request->content_type = "image/png";
    }
    if(0 == filename.substr(len - 4, 4).compare(".gif")){
        request->content_type = "image/gif";
    }
    if(0 == filename.substr(len - 4, 4).compare(".jpg")){
        request->content_type = "image/jpg";
    }
    if(0 == filename.substr(len - 4, 4).compare(".css")){
        request->content_type = "text/css";
    }
}

WebFileFetcher::~WebFileFetcher()
{
}

int WebFileFetcher::tick()
{
    request->client->send_command("HTTP/1.0 200 OK");
    request->client->send_command("Content-type: " + request->content_type + "\n");
    request->client->send_data(buf, size);
    return 0;
}
