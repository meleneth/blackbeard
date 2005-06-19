#ifndef ___uu_decoder_inc
#define ___uu_decoder_inc 1

#include"SDL.h"
#include"decoder.hpp"

class UUDecoder : public Decoder {
    public:
        // Public data members go here.
        UUDecoder(int bar); // Constructor
        ~UUDecoder(); // Destructor
        int Foo(void);
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
