#ifndef ___webpostsets_inc
#define ___webpostsets_inc 1

#include "SDL_types.h"
#include "webdatafetcher.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;

class WebPostSets : public WebDataFetcher {
    public:
        WebPostSets(WebRequest *request);
        virtual ~WebPostSets();
    private:
    protected:
};

#endif