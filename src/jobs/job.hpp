#ifndef ___job_inc
#define ___job_inc 1

#include "SDL_types.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

typedef enum { MISC, POSTSET_DOWNLOAD, BODY_DOWNLOAD } JobType;

class Job;
class Job {
    public:
        Job();
        virtual ~Job();
        virtual void process();
        virtual string status_line();
        virtual void notify_child_finished(Job *finished_job);
        virtual bool has_more_fruit();
        virtual void shake_tree();
        static bool compare(const Job* a, const Job* b);
        virtual void add_child_job(Job *job);
        virtual void finish(void);

        Uint32 priority;
        Uint32 is_finished;
        void *srv;
        string _status;
        JobType job_type;
        string job_status_filename;
        Job *parent;
        vector<Job *> children;

    private:
    protected:
};


#endif
