#ifndef ___postsetjob_inc
#define ___postsetjob_inc 1

#include"SDL_types.h"
#include"job.hpp"
#include"post_set.hpp"

#include<string>
using std::string;

class PostsetJob : public Job {
    public:
        PostsetJob(PostSet* post_set);
        PostsetJob(string filename);
        virtual ~PostsetJob();

        Uint32 pieces_left_to_download();
        virtual void process();
        virtual string status_line();
        virtual void notify_child_finished(Job *job);
        void save_job_status();
        void load_job_status();
        void make_downloader_children();

        Job *job;
        Job *get_headers;
        PostSet *postset;
        Uint32 file_no;
        Uint32 piece_no;
        Uint32 max_file_no;
        Uint32 max_piece_no;
    private:
    protected:
};

#endif
