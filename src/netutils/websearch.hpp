#ifndef ___websearch_inc
#define ___websearch_inc 1

#include "SDL_types.h"
#include "webdatafetcher.hpp"

class WebSearch : public WebDataFetcher {
    public:
        WebSearch(WebRequest *request);
        virtual ~WebSearch();

        virtual string info_update_string(void);
        void search(vector<PostSet>& results, const string& needle);

    private:
    protected:
};

#endif
