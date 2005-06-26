#include <sys/types.h>
#include <sys/stat.h>
#include "yenc_decoder.hpp"
#include"globals.hpp"
#include<stdio.h>

// Public data members go here.
yEncDecoder::yEncDecoder() :Decoder()// Constructor
{
    status = S_MESSAGE;

//=ybegin line=128 size=123456 name=mybinary.dat
    header_pattern = new StringPattern();
    header_pattern->add_breaker("=ybegin line=");
    header_pattern->add_breaker(" size=");
    header_pattern->add_breaker(" name=");

//=ypart begin=1 end=100000
    part_pattern = new StringPattern();
    part_pattern->add_breaker("=ypart begin=");
    part_pattern->add_breaker(" end=");

    footer_pattern = new StringPattern();
    footer_pattern->add_breaker("yend");
}
    
yEncDecoder::~yEncDecoder() // Destructor
{
}

void yEncDecoder::decode_line(string line)
{
    if(S_MESSAGE == status){
        if(header_pattern->does_match(line)){
            console->log("yBegin header found");
            status = S_BODY;
            
        } else {
            if(part_pattern->does_match(line)){
                console->log("yPart header found");
                status = S_BODY;
            }
        }
    }else{
        string footer="=yend";
        if(0 == footer.compare(line.substr(0, footer.length()))){
            console->log("yEnd footer found");
            status = S_MESSAGE;
        } else {
            fwrite(line.c_str(), 1, line.length(), fileptr);
        }
    }
}

// Private members go here.
// Protected members go here.
