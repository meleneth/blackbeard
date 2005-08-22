#ifndef ___bodyretrieverjob_inc
#define ___bodyretrieverjob_inc 1

#include"SDL_types.h"
#include"netjob.hpp"
#include"newsgroup.hpp"
#include"post_file.hpp"

#include<string>
using std::string;

class BodyRetrieverJob : public NetJob {
    public:
        BodyRetrieverJob(NewsGroup *group, PostFile *file, Uint32 msg_id);
        virtual ~BodyRetrieverJob();

        virtual void process_line(string line);
        virtual void finish();
        virtual string status_line();

        NewsGroupPost *post;
        PostFile *file;
        Uint32 msg_id;
    private:
    protected:
};

#endif
