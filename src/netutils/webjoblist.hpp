#ifndef ___webjoblist_inc
#define ___webjoblist_inc 1

#include "SDL_types.h"
#include "webdatafetcher.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;

class WebJobList : public WebDataFetcher {
    public:
        WebJobList(WebRequest *request);
        virtual ~WebJobList();

        virtual int tick(void);
        vector<string> job_list;
        Uint32 job_no;
        Uint32 num_jobs;
    private:
    protected:
};

#endif
