#ifndef ___decoder_inc
#define ___decoder_inc 1

#include"SDL_types.h"
#include<string>
#include"newsgrouppost.hpp"

using std::string;

class Decoder {
    public:
        // Public data members go here.
        Decoder(); // Constructor
        virtual ~Decoder(); // Destructor

        virtual void decode(NewsGroupPost *newsgrouppost, string filename);
        virtual void decode_line(string line);

        string filename;
        FILE *fileptr;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
