#include "thread.hpp"
#include"console.hpp"

// Public data members go here.
Thread::Thread() // Constructor
{
}
    
Thread::~Thread() // Destructor
{
}


void Thread::Start(void)
{
    console->log("Spawning thread..");
    ThreadId = pth_spawn(PTH_ATTR_DEFAULT, Thread::EntryPoint, this);
}

/*static */
void *Thread::EntryPoint(void *pthis)
{
   Thread *pt = (Thread*)pthis;
   console->log("Calling Execute()");
   pt->Execute();
   return NULL;
}

void Thread::Execute(void)
{
   console->log("Thread::Execute() called");
        // Your code goes here
}



// Private members go here.
// Protected members go here.
