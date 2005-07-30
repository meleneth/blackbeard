#ifndef ___xoverjob_inc
#define ___xoverjob_inc 1

#include"SDL_types.h"
#include"job.hpp"

#include<string>

using std::string;


#define XOVER_CHUNK_SIZE 5000

class XoverJob : public Job {
    public:
        XoverJob();
        virtual ~XoverJob();
        virtual void process();

        string group_name;
        Uint32 lower_id;
        Uint32 upper_id;
    private:
    protected:
};

#endif
