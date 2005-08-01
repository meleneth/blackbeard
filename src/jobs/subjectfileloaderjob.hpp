#ifndef ___subjectfileloaderjob_inc
#define ___subjectfileloaderjob_inc 1

#include"SDL_types.h"
#include"job.hpp"
#include<string>

using std::string;

class SubjectFileLoaderJob : public Job {
    public:
        SubjectFileLoaderJob(string filename);
        ~SubjectFileLoaderJob();

        virtual void process(void);
        string filename;
        virtual string status_line();

        Uint32 total_bytes;
        Uint32 bytes_read;

    private:
    protected:
};

#endif
