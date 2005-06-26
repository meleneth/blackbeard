#ifndef ___yenc_decoder_inc
#define ___yenc_decoder_inc 1

#include"SDL_types.h"
#include"decoder.hpp"
#include"newsgrouppost.hpp"
#include"stringpattern.hpp"

using std::string;

typedef enum yEnc_STATUS { S_MESSAGE, S_BODY };

class yEncDecoder : public Decoder {
    public:
        yEncDecoder(); 
        virtual ~yEncDecoder(); 

        virtual void decode_line(string line);

        enum yEnc_STATUS status;
        StringPattern *header_pattern;
        StringPattern *part_pattern;
        StringPattern *footer_pattern;
    private:
    protected:
};

#endif
