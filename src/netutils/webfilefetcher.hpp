#ifndef ___webfilefetcher_inc
#define ___webfilefetcher_inc 1

#include "SDL_types.h"
#include "webdatafetcher.hpp"
#include <string>

using std::string;

class WebFileFetcher : public WebDataFetcher{
    public:
        WebFileFetcher(WebRequest *request, string filename);
        virtual ~WebFileFetcher();

    private:
    protected:
};

#endif
