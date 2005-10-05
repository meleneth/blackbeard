#ifndef ___job_inc
#define ___job_inc 1

#include"SDL_types.h"
#include<string>

using std::string;

typedef enum { MISC, POSTSET_DOWNLOAD, BODY_DOWNLOAD } JobType;

class Job {
    public:
        Job();
        virtual ~Job();
        virtual void process();
        virtual string status_line();
        static bool compare(const Job* a, const Job* b);

        Uint32 priority;
        Uint32 is_finished;
        void *srv;
        string _status;
        JobType job_type;
        string job_status_filename;

    private:
    protected:
};


#endif
