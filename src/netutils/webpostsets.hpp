#ifndef ___webpostsets_inc
#define ___webpostsets_inc 1

#include "SDL_types.h"
#include "webdatafetcher.hpp"
#include "newsgroup.hpp"
#include "post_set.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;

class WebPostSets : public WebDataFetcher {
    public:
        WebPostSets(WebRequest *request);
        virtual ~WebPostSets();
        virtual string info_update_string(void);
        string status(PostSet *set);

        NewsGroup *group;
    private:
    protected:
};

#endif
