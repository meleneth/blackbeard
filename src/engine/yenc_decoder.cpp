#include <sys/types.h>
#include <sys/stat.h>
#include "yenc_decoder.hpp"
#include<stdio.h>

// Public data members go here.
yEncDecoder::yEncDecoder() :Decoder()// Constructor
{
    status = S_MESSAGE;
}
    
yEncDecoder::~yEncDecoder() // Destructor
{
}

void yEncDecoder::decode_line(string line)
{
    if(S_MESSAGE == status){
        string header = "=ybegin";
        if(0 == header.compare(line.substr(0, header.length()))){
            status = S_BODY;
        } else {
            header = "=ypart";
            if(0 == header.compare(line.substr(0, header.length()))){
                status = S_BODY;
            }
        }
    }else{
        string footer="=yend";
        if(0 == footer.compare(line.substr(0, footer.length()))){
            status = S_MESSAGE;
        } else {
            fwrite(line.c_str(), 1, line.length(), fileptr);
        }
    }
}

// Private members go here.
// Protected members go here.
