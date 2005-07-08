#ifndef ___uu_decoder_inc
#define ___uu_decoder_inc 1

#include"SDL_types.h"
#include"decoder.hpp"
#include"post_file.hpp"

class UUDecoder : public Decoder {
    public:
        UUDecoder(NewsGroupPost *newsgrouppost, PostFile *file, string message_id); 
        virtual ~UUDecoder();

        virtual void decode_line(string line);
        virtual void open_file();
        string do_the_math(string line);
        Uint32 num_encoded_chars;

    private:
    protected:
};

#endif
