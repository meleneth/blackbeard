#ifndef ___headersforgroupjob_inc
#define ___headersforgroupjob_inc 1

#include"SDL_types.h"
#include"netjob.hpp"
#include"newsgroup.hpp"
#include"config.hpp"
#include <iostream>  // I/O 
#include<fstream>
using std::ofstream;

class HeadersForGroupJob : public NetJob {
    public:
        HeadersForGroupJob(NewsGroup *group);
        virtual ~HeadersForGroupJob();

        virtual void process_line(string line);
        virtual string status_line();

        NewsGroup *group;
        ofstream out;
    private:
    protected:
};

#endif
