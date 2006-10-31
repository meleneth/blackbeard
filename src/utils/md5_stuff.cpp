#include "md5_stuff.hpp"
#include <iomanip>

#include <sstream>

#include "console.hpp"

using std::stringstream;

void hex_to_hash(unsigned char *hash, string hex)
{
    char buf[3];
    buf[2] = 0;
    unsigned int r;

    Uint32 i;
    for(i=0; i<16; ++i){
        buf[0] = hex[2*i];
        buf[1] = hex[(2*i)+1];
        sscanf(buf, "%x", &r);
        unsigned char x = r;
        *(hash + i) = x;
    }
}

string hash_to_hex(unsigned char *hash)
{
    stringstream result;
    result << std::hex;

    Uint32 i;
    for(i=0; i<16; ++i){
        unsigned char x = *(hash + i);
        if(x < 10)
            result << 0;
        result << (unsigned short int)x;
    }
    return result.str();
}


