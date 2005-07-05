#ifndef ___jobqueue_inc
#define ___jobqueue_inc 1

#include"SDL_types.h"
#include"decoder.hpp"
#include<list>

using std::list;

class JobQueue {
    public:
        JobQueue();
        ~JobQueue(); 

        Decoder *get_next_decoder_job(void);
        void add_decoder_job(Decoder *job);

        list<Decoder *> decoder_jobs;
    private:
    protected:
};

#endif
