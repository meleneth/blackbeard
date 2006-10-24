#ifndef ___postfilejob_inc
#define ___postfilejob_inc 1

#include"SDL_types.h"
#include"job.hpp"
#include"post_set.hpp"

#include<string>
using std::string;

class PostfileJob : public Job {
    public:
        PostfileJob(PostFile* post_file);
        virtual ~PostfileJob();

        Job *get_next_job();
        virtual void process();
        virtual string status_line();

        Job *job;
        PostFile *postfile;
        list<FilePiece *>::iterator piece;
        Uint32 max_file_no;
        Uint32 max_piece_no;
    private:
    protected:
};

#endif
