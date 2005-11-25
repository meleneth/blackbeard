#ifndef ___webdatafetcher_inc
#define ___webdatafetcher_inc 1

#include "SDL_types.h"
#include "webrequest.hpp"

class WebDataFetcher {
    public:
        WebDataFetcher(WebRequest *request);
        virtual ~WebDataFetcher();

        virtual int tick(void);

        WebRequest *request;

    private:
    protected:
};

#endif
