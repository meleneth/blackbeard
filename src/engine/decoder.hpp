#ifndef ___decoder_inc
#define ___decoder_inc 1

#include"SDL_types.h"
#include<string>

using std::string;

class Decoder {
    public:
        // Public data members go here.
        Decoder(); // Constructor
        ~Decoder(); // Destructor

        int Foo(void);

        string filename;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
