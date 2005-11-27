#ifndef ___webjoblist_inc
#define ___webjoblist_inc 1

#include "SDL_types.h"
#include "webdatafetcher.hpp"
#include "job.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;

class WebJobList : public WebDataFetcher {
    public:
        WebJobList(WebRequest *request);
        virtual ~WebJobList();
        string line_for_job(Job *job);

    private:
    protected:
};

#endif
