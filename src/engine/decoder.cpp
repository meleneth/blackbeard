#include "decoder.hpp"
#include <sys/types.h>
#include <sys/stat.h>



// Public data members go here.
Decoder::Decoder() // Constructor
{
}
    
Decoder::~Decoder() // Destructor
{
    if(post){
        delete post;
    }
}

void Decoder::decode()
{
    list<string>::iterator s;
    
    open_file();
    for (s=post->lines.begin(); s!=post->lines.end(); ++s){
        decode_line(*s);
    }
    close_file();
}

void Decoder::open_file(void)
{
    struct stat my_stats;
    if(stat(filename.c_str(), &my_stats) == -1){
        fileptr = fopen(filename.c_str(), "w");
    } else {
        fileptr = fopen(filename.c_str(), "r+");
    }
}

void Decoder::close_file(void)
{
    fclose(fileptr);
}

void Decoder::decode_line(string line)
{
}

// Private members go here.
// Protected members go here.
