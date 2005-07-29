#ifndef ___job_inc
#define ___job_inc 1

#include"SDL_types.h"

class Job {
    public:
        Job();
        virtual ~Job();
	virtual void process();

    Uint32 priority;

    private:
    protected:
};

#endif
