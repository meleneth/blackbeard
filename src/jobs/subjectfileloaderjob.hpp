#ifndef ___subjectfileloaderjob_inc
#define ___subjectfileloaderjob_inc 1

#include"SDL_types.h"
#include"job.hpp"
#include<string>
#include"newsgroup.hpp"
#include <iostream>  // I/O 
#include <fstream>   // file I/O
#include<sstream>

using std::string;
using std::ifstream;
using std::ios;

class SubjectFileLoaderJob : public Job {
    public:
        SubjectFileLoaderJob(string filename);
        ~SubjectFileLoaderJob();

        virtual void process(void);
        string filename;
        virtual string status_line();

        Uint32 total_bytes;
        Uint32 bytes_read;

        NewsGroup *newsgroup;
        char linebuffer[1024];
        ifstream in;
    private:
    protected:
};

#endif
