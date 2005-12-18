#ifndef ___jobparent_inc
#define ___jobparent_inc 1

#include "SDL_types.h"
#include "job.hpp"
#include <vector>

using std::vector;

class Job;
class JobParent {
    public:
        JobParent();
        virtual ~JobParent();

        virtual void notify_child_finished(Job *finished_job);
        virtual void add_child_job(Job *job);
        vector<Job *> children;
    private:
    protected:
};

#endif
