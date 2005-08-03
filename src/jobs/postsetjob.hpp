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
        virtual ~PostsetJob();

        Job *get_next_job();
        Uint32 pieces_left_to_download();
        virtual void process(void *connection);
        virtual string status_line();

        PostSet *postset;
    private:
    protected:
};

#endif
