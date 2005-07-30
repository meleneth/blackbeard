#ifndef ___bodyretrieverjob_inc
#define ___bodyretrieverjob_inc 1

#include"SDL_types.h"
#include"job.hpp"

class BodyRetrieverJob : public Job {
    public:
        BodyRetrieverJob();
        virtual ~BodyRetrieverJob();

        virtual void process(void *connection);;

    private:
    protected:
};

#endif
