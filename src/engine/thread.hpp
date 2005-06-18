#ifndef ___thread_inc
#define ___thread_inc 1

#include"SDL_types.h"
#include"pthread.h"

class Thread {
    public:
        // Public data members go here.
        Thread(); // Constructor
        virtual ~Thread(); // Destructor

        void Start(void);
        static void *EntryPoint(void*);
        virtual void Execute(void);

        pthread_t ThreadId;
            // Private members go here.
    protected:
            // Protected members go here.
};

#endif
