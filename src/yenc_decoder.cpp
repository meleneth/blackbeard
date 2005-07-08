#include <sys/types.h>
#include <sys/stat.h>
#include "yenc_decoder.hpp"
#include"globals.hpp"
#include<stdio.h>

#define ASCII_NULL  0x00 
#define ASCII_LF    0x0A
#define ASCII_CR    0x0D
#define ASCII_EQ    0x3D     
//
// Public data members go here.
yEncDecoder::yEncDecoder(NewsGroupPost *newsgrouppost, string filename) :Decoder()// Constructor
{
    status = S_MESSAGE;
    this->filename = filename;
    this->post = newsgrouppost;

//=ybegin line=128 size=123456 name=mybinary.dat
    header_pattern = new StringPattern(3);
    header_pattern->add_breaker("=ybegin line=");
    header_pattern->add_breaker(" size=");
    header_pattern->add_breaker(" name=");

//=ypart begin=1 end=100000
    part_pattern = new StringPattern(2);
    part_pattern->add_breaker("=ypart begin=");
    part_pattern->add_breaker(" end=");

    footer_pattern = new StringPattern(1);
    footer_pattern->add_breaker("yend");
}
    
yEncDecoder::~yEncDecoder() // Destructor
{
    delete header_pattern;
    delete part_pattern;
    delete footer_pattern;
}

string yEncDecoder::do_the_math(string line)
{
    size_t index;
    for (index = 0; index < line.length(); ++index){
        if (ASCII_EQ == line[index]){
            line.erase(index, 1);
            line[index] -= 64;
        }
        line[index] -= 42;
    }
    return line;
}
void yEncDecoder::decode_line(string line)
{
    if(S_MESSAGE == status){
        if(header_pattern->does_match(line)){
            status = S_BODY;
            open_file();
        } else {
            if(part_pattern->does_match(line)){
                status = S_BODY;
                part_pattern->pieces(line);
                open_file();
                fseek(fileptr, atoi(part_pattern->results[1].c_str()) -1, SEEK_SET);
            }
        }
    }else{
        string footer="=yend";
        if(0 == footer.compare(line.substr(0, footer.length()))){
            status = S_MESSAGE;
        } else {
            string decoded_line = do_the_math(line);
            fwrite(decoded_line.c_str(), 1, decoded_line.length(), fileptr);
        }
    }
}



// Private members go here.
// Protected members go here.
