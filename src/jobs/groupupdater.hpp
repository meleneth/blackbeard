#ifndef ___groupupdater_inc
#define ___groupupdater_inc 1

#include "SDL_types.h"
#include "netjob.hpp"

class GroupUpdater : public Job {
    public:
        GroupUpdater(NewsGroup *group);
        ~GroupUpdater();

        virtual void process(void);
        NewsGroup *group;
        Job *sub_job;

        Uint32 sent_command;

    private:
    protected:
};

#endif
