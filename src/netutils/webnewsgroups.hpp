#ifndef ___webnewsgroups_inc
#define ___webnewsgroups_inc 1

#include "SDL_types.h"
#include "webdatafetcher.hpp"
#include "newsgroup.hpp"
#include <string>
#include <vector>
#include "webrequest.hpp"

using std::string;
using std::vector;

class WebNewsGroups : public WebDataFetcher {
    public:
        WebNewsGroups(WebRequest *request);
        virtual ~WebNewsGroups();
        string status(NewsGroup *group, Uint32 index);

        virtual string info_update_string(void);

    private:
    protected:
};

#endif
