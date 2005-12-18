#ifndef ___webviewfile_inc
#define ___webviewfile_inc 1

#include "SDL_types.h"
#include "webdatafetcher.hpp"
#include "post_file.hpp"
#include "jobparent.hpp"

class WebViewFile : public WebDataFetcher, public JobParent  {
    public:
        WebViewFile(WebRequest *request);
        ~WebViewFile();

        virtual int tick(void);
        virtual void notify_child_finished(Job *job);
        void read_file(void);

        PostFile *file;

    private:
    protected:
};

#endif
