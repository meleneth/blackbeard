#ifndef ___netjob_inc
#define ___netjob_inc 1

#include"SDL_types.h"
#include<string>
#include"job.hpp"

#define MAX_NETJOB_LINES_PER_SLICE 5000

using std::string;

// This handles a multi-line response generic network job

class NetJob : public Job {
    public:
        NetJob();
        ~NetJob();
        virtual void process(void);
        virtual void process_line(string line);

        string net_cmd;
        Uint32 sent_command;
    private:
    protected:
};

#endif
