#ifndef ___netthread_inc
#define ___netthread_inc 1

#include"SDL_types.h"
#include"thread.hpp"
#include"nntpserver.hpp"
#include"config.hpp"
#include"post_set.hpp"

class NetThread : public Thread {
    public:
        // Public data members go here.
        NetThread(Config *cfg); // Constructor
        ~NetThread(); // Destructor
        virtual void Execute(void);
        void retrieve(PostSet *postset);

        Config *config;
        NNTPServer *connection;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
