#ifndef ___activejobsscreen_inc
#define ___activejobsscreen_inc 1

#include"SDL_types.h"
#include"screen.hpp"

class ActiveJobsScreen : public Screen {
    public:
        ActiveJobsScreen(int bar);
        ~ActiveJobsScreen();

        virtual void render(void);

    private:
    protected:
};

#endif
