#include "decoderfactory.hpp"
#include "newsgrouppost.hpp"
#include "console.hpp"

Decoder *decoder_for_body(NewsGroupPost *post, PostFile *file)
{
    StringPattern  yenc_pattern = StringPattern(2);
    yenc_pattern.add_breaker("=ybegin");

    Uint32 max_no = 10;
    if(max_no > post->lines.size())
        max_no = post->lines.size();

    for(Uint32 i=0; i<max_no; ++i){
        if(yenc_pattern.match((*post).lines[i]))
            return new yEncDecoder(post, file);
    }
    console->log("Could not determine encoded data type");
    return NULL;
}

