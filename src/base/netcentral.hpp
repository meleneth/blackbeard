#ifndef ___netcentral_inc
#define ___netcentral_inc 1

#include"SDL_types.h"

#include"nntpserver.hpp"
#include"jobqueue.hpp"
#include<vector>

using std::vector;

class NetCentral : public JobQueue {
    public:
        // Public data members go here.
        NetCentral(void); // Constructor
        virtual ~NetCentral(); // Destructor

        virtual void process_jobs(void);
        void add_connection(NNTPServer *connection);

        vector <NNTPServer *> connections;
        int fdmax;

        fd_set master;   // master file descriptor list
        fd_set read_fds; // temp file descriptor list for select()
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

extern NetCentral *netcentral;

#endif
