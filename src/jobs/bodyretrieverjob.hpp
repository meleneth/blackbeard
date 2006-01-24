#ifndef ___bodyretrieverjob_inc
#define ___bodyretrieverjob_inc 1

#include"SDL_types.h"
#include"netjob.hpp"
#include"post_file.hpp"

#include<string>
using std::string;

class BodyRetrieverJob : public NetJob {
    public:
        BodyRetrieverJob(PostFile *file, FilePiece *piece);
        virtual ~BodyRetrieverJob();

        virtual void process_line(string line);
        virtual void finish();
        virtual string status_line();

        NewsGroupPost *post;
        PostFile *file;
        FilePiece *piece;
    private:
    protected:
};

#endif
