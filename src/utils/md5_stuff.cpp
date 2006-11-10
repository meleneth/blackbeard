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
    string result;
    char buf[3];
    buf[3] = '\0';

    Uint32 i;
    for(i=0; i<16; ++i){
        stringstream s;
        s << " Iter: " << i << " || " << result;
//        console->log(s.str());
        sprintf(buf, "%02x", *(hash+i));
        result.append(buf);
    }
    return result;
}


