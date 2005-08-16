#ifndef ___xoverjob_inc
#define ___xoverjob_inc 1

#include"SDL_types.h"
#include"job.hpp"
#include"newsgroup.hpp"

#include<string>

using std::string;


#define XOVER_CHUNK_SIZE 5000

class XoverJob : public Job {
    public:
        XoverJob(NewsGroup *group, Uint32 lower, Uint32 upper);
        virtual ~XoverJob();
        virtual void process(void *connection);
        virtual string status_line(void);

        NewsGroup *group;
        Uint32 lower_id;
        Uint32 upper_id;
    private:
    protected:
};

#endif
