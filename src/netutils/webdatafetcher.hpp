#ifndef ___webdatafetcher_inc
#define ___webdatafetcher_inc 1

#include "SDL_types.h"
#include "webrequest.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;

class WebDataFetcher {
    public:
        WebDataFetcher(WebRequest *request);
        virtual ~WebDataFetcher();

        virtual int tick(void);
        virtual string info_update_string(void);

        WebRequest *request;

        vector<string> output_lines;
        Uint32 line_no;
        Uint32 num_lines;
        string title;

    private:
    protected:
};

#endif
