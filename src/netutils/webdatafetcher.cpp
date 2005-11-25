#include "webdatafetcher.hpp"

WebDataFetcher::WebDataFetcher(WebRequest *r)
{
    this->request = r;
}

WebDataFetcher::~WebDataFetcher()
{
}

int WebDataFetcher::tick(void)
{
    return 0;
}

