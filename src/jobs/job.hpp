#ifndef ___job_inc
#define ___job_inc 1

#include"SDL_types.h"

class Job {
    public:
        Job();
        ~Job();
	Uint32 type();
	void process();

    private:
    protected:
};

#endif
