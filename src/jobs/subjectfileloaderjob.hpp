#ifndef ___subjectfileloaderjob_inc
#define ___subjectfileloaderjob_inc 1

#include"SDL_types.h"
#include"job.hpp"

class SubjectFileLoaderJob : public Job {
    public:
        SubjectFileLoaderJob();
        ~SubjectFileLoaderJob();

        virtual void process(void);

    private:
    protected:
};

#endif
