#ifndef ___yenc_decoder_inc
#define ___yenc_decoder_inc 1

#include"SDL_types.h"
#include"decoder.hpp"

class yEncDecoder : public Decoder {
    public:
        // Public data members go here.
        yEncDecoder(int bar); // Constructor
        ~yEncDecoder(); // Destructor
        int Foo(void);
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
