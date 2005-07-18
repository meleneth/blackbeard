#ifndef ___input_thread_inc
#define ___input_thread_inc 1

#include"SDL_types.h"
#include"thread.hpp"
#include"net_thread.hpp"

class InputThread : public Thread {
    public:
        InputThread(NetThread *thread);
        virtual ~InputThread(); 
        virtual void Execute(void);
        
        NetThread *net_thread;
    private:
    protected:
};

#endif
