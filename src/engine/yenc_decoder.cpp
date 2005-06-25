#include "yenc_decoder.hpp"
#include<stdio.h>

// Public data members go here.
yEncDecoder::yEncDecoder() :Decoder()// Constructor
{
}
    
yEncDecoder::~yEncDecoder() // Destructor
{
}

void yEncDecoder::decode(NewsGroupPost *newsgrouppost)
{
    FILE *fh = fopen("newsgrouppost", "w");
    fclose(fh);
}

// Private members go here.
// Protected members go here.
