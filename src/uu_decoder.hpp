#ifndef ___uu_decoder_inc
#define ___uu_decoder_inc 1

#include"SDL_types.h"
#include"decoder.hpp"

class UUDecoder : public Decoder {
    public:
        UUDecoder(NewsGroupPost *newsgrouppost, Uint32 piece_no); 
        virtual ~UUDecoder();

        virtual void decode_line(string line);
        virtual void open_file();

        Uint32 piece_no;
    private:
    protected:
};

#endif
