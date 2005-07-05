#ifndef ___yenc_decoder_inc
#define ___yenc_decoder_inc 1

#include"SDL_types.h"
#include"decoder.hpp"
#include"newsgrouppost.hpp"
#include"stringpattern.hpp"
#include<string>

using std::string;


class yEncDecoder : public Decoder {
    public:
        yEncDecoder(NewsGroupPost *newsgrouppost, string filename); 
        virtual ~yEncDecoder(); 

        virtual void decode_line(string line);
        string do_the_math(string line);

    private:
    protected:
};

#endif
