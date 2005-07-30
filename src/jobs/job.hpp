#ifndef ___job_inc
#define ___job_inc 1

#include"SDL_types.h"

class Job {
    public:
        Job();
        virtual ~Job();
        virtual void process();
        virtual void process(void *connection);

    Uint32 priority;
    Uint32 is_finished;

    private:
    protected:
};

#endif
