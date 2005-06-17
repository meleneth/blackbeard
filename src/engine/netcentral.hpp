#ifndef ___netcentral_inc
#define ___netcentral_inc 1

#include<list>
#include"SDL_types.h"

#include"nntpserver.hpp"

class NetCentral {
    public:
        // Public data members go here.
        NetCentral(void); // Constructor
        ~NetCentral(); // Destructor

        void tick(void);
        void add_connection(NNTPServer *connection);

        std::list<NNTPServer *> connections;
        int fdmax;

        fd_set master;   // master file descriptor list
        fd_set read_fds; // temp file descriptor list for select()
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
