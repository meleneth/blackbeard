#ifndef ___postsetjob_inc
#define ___postsetjob_inc 1

#include"SDL_types.h"
#include"job.hpp"
#include"post_set.hpp"

class PostsetJob : public Job {
    public:
        PostsetJob(PostSet* post_set);
        ~PostsetJob();
	Job* get_next_job();
	Uint32 pieces_left_to_download();
	Uint32 type();

	PostSet *postset;

    private:
    protected:
};

#endif
