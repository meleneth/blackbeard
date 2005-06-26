#include <sys/types.h>
#include <sys/stat.h>
#include "yenc_decoder.hpp"
#include<stdio.h>

// Public data members go here.
yEncDecoder::yEncDecoder() :Decoder()// Constructor
{
}
    
yEncDecoder::~yEncDecoder() // Destructor
{
}

void yEncDecoder::decode(NewsGroupPost *newsgrouppost, string filename)
{
    struct stat my_stats;
    FILE *fh;

    if(stat(filename.c_str(), &my_stats) == -1){
        fh = fopen(filename.c_str(), "w");
    } else {
        fh = fopen(filename.c_str(), "r+");
    }
    fclose(fh);
}

// Private members go here.
// Protected members go here.
