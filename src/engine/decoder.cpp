#include "decoder.hpp"
#include <sys/types.h>
#include <sys/stat.h>



// Public data members go here.
Decoder::Decoder() // Constructor
{
}
    
Decoder::~Decoder() // Destructor
{
}

void Decoder::decode(NewsGroupPost *newsgrouppost, string filename)
{
    struct stat my_stats;
    list<string>::iterator s;
    
    if(stat(filename.c_str(), &my_stats) == -1){
        fileptr = fopen(filename.c_str(), "w");
    } else {
        fileptr = fopen(filename.c_str(), "r+");
    }
    for (s=newsgrouppost->lines.begin(); s!=newsgrouppost->lines.end(); ++s){
        decode_line(*s);
    }
    fclose(fileptr);
}

void Decoder::decode_line(string line)
{
}

// Private members go here.
// Protected members go here.
