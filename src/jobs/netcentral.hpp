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
        Uint32 krate(void);
        void save_active_list_file();
        void restore_saved_jobs();
        virtual void add_job(Job *job);
        virtual void finish_job(Job *job);

        int fdmax;

        fd_set master;   // master file descriptor list
        fd_set read_fds; // temp file descriptor list for select()
  
        vector<string> job_filenames; 
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

extern NetCentral *netcentral;

#endif