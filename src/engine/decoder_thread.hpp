#ifndef ___decoder_thread_inc
#define ___decoder_thread_inc 1

#include"SDL_types.h"
#include"thread.hpp"
#include"decoder.hpp"

class DecoderThread : public Thread {
    public:
        // Public data members go here.
        DecoderThread(); // Constructor
        ~DecoderThread(); // Destructor
        virtual void Execute(void);
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
