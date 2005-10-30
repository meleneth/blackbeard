#ifndef ___groupupdater_inc
#define ___groupupdater_inc 1

#include "SDL_types.h"
#include "netjob.hpp"

#include <string>
using std::string;

class GroupUpdater : public Job {
    public:
        GroupUpdater(NewsGroup *group);
        ~GroupUpdater();

        virtual void process(void);
        void update_group(string line);

        NewsGroup *group;
        Uint32 sent_command;
    private:
    protected:
};

#endif
