#include "decoder.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include"globals.hpp"



// Public data members go here.
Decoder::Decoder() // Constructor
{
}
    
Decoder::~Decoder() // Destructor
{
    if(post){
        delete post;
    }
    close_file();
}

void Decoder::decode()
{
    list<string>::iterator s;
    
    for (s=post->lines.begin(); s!=post->lines.end(); ++s){
        decode_line(*s);
    }
}

void Decoder::open_file(void)
{
    console->log("Opening [" + filename + "] for writing");
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
