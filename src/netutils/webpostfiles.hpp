#ifndef ___webpostfiles_inc
#define ___webpostfiles_inc 1

#include "SDL_types.h"
#include "webdatafetcher.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;

class WebPostFiles : public WebDataFetcher {
    public:
        WebPostFiles(WebRequest *request);
        virtual ~WebPostFiles();
    private:
    protected:
};

#endif
