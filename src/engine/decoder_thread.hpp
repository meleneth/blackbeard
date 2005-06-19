#ifndef ___decoder_thread_inc
#define ___decoder_thread_inc 1

#include"SDL_types.h"
#include"thread.hpp"
#include"decoder.hpp"

class DecoderThread : public Thread {
    public:
        // Public data members go here.
        DecoderThread(int bar); // Constructor
        ~DecoderThread(); // Destructor
        int Foo(void);
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
