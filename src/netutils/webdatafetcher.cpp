#include "webdatafetcher.hpp"

WebDataFetcher::WebDataFetcher(WebRequest *r)
{
    this->request = r;
    line_no = 0;
    num_lines = 0;
}

WebDataFetcher::~WebDataFetcher()
{
    delete request;
}

int WebDataFetcher::tick(void)
{
    if(num_lines)
        request->client->send_command(output_lines[line_no]);
    return ++line_no < num_lines;
}

