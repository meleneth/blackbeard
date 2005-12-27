#ifndef ___webpostfiles_inc
#define ___webpostfiles_inc 1

#include "SDL_types.h"
#include "webdatafetcher.hpp"
#include "post_set.hpp"

#include <string>
#include <vector>

using std::string;
using std::vector;

class WebPostFiles : public WebDataFetcher {
    public:
        WebPostFiles(WebRequest *request);
        virtual ~WebPostFiles();

        virtual string info_update_string(void);
        string post_file_line(PostFile *file, Uint32 file_index);
        Uint32 file_is_viewable(string filename);

        PostSet *set;
    private:
    protected:
};

#endif
