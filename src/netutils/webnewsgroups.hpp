#ifndef ___webnewsgroups_inc
#define ___webnewsgroups_inc 1

#include "SDL_types.h"
#include "webdatafetcher.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;

class WebNewsGroups : public WebDataFetcher {
    public:
        WebNewsGroups(WebRequest *request);
        virtual ~WebNewsGroups();

    private:
    protected:
};

#endif
