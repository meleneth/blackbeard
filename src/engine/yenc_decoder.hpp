#ifndef ___yenc_decoder_inc
#define ___yenc_decoder_inc 1

#include"SDL_types.h"
#include"decoder.hpp"

class yEncDecoder : public Decoder {
    public:
        yEncDecoder(); 
        virtual ~yEncDecoder(); 

        virtual void decode(void);
    private:
    protected:
};

#endif
