#ifndef ___uu_decoder_inc
#define ___uu_decoder_inc 1

#include"SDL_types.h"
#include"decoder.hpp"

class UUDecoder : public Decoder {
    public:
        UUDecoder(); 
        virtual ~UUDecoder();

        virtual void decode_line(string line);
    private:
    protected:
};

#endif
