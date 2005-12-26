#ifndef ___webfilefetcher_inc
#define ___webfilefetcher_inc 1

#include "SDL_types.h"
#include "webdatafetcher.hpp"
#include <string>

using std::string;

class WebFileFetcher : public WebDataFetcher {
    public:
        WebFileFetcher(WebRequest *request, string filename);
        virtual ~WebFileFetcher();
        virtual int tick(void);

        char *buf;
        Uint32 size;
        Uint32 index;
    private:
    protected:
};

#endif
