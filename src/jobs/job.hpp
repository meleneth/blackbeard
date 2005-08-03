#ifndef ___job_inc
#define ___job_inc 1

#include"SDL_types.h"
#include<string>

using std::string;

class Job {
    public:
        Job();
        virtual ~Job();
        virtual void process();
        virtual void process(void *connection);
        virtual string status_line();

    Uint32 priority;
    Uint32 is_finished;

    private:
    protected:
};

void lock_jobs(void);
void unlock_jobs(void);

#endif
