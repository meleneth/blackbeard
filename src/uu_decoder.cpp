#include "uu_decoder.hpp"

#include<sstream>
using std::stringstream;
#include"globals.hpp"

/* piece files are 7500 lines
 * lines hold 45 bytes each (decoded)
 * which comes out to 337,500 bytes encoded per uuencoded piece file
 */

#define UUDECODER_OFFSET 337500

// Public data members go here.
UUDecoder::UUDecoder(NewsGroupPost *newsgrouppost, PostFile *file, Uint32 piece_no) : Decoder()// Constructor
{
    this->piece_no = piece_no;
    this->post = newsgrouppost;
    this->file = file;
    status = S_MESSAGE;

    header_pattern = new StringPattern(3);
    header_pattern->add_breaker("begin ");
    header_pattern->add_breaker(" ");
}
    
UUDecoder::~UUDecoder() // Destructor
{
}

void UUDecoder::open_file(void)
{
    Decoder::open_file();

    size_t seek_offset =  UUDECODER_OFFSET * (piece_no - 1);
    stringstream buf;
    buf << "Seek offset: " << seek_offset;
    console->log(buf.str());

//    fseek(fileptr, seek_offset, SEEK_SET);
}

void UUDecoder::decode_line(string line)
{
    
    if(S_MESSAGE == status){
        if(header_pattern->does_match(line)){
            status = S_BODY;
        }
    }else if(S_BODY == status){
        string decoded_line = do_the_math(line);
        fwrite(decoded_line.c_str(), 1, decoded_line.length(), fileptr);
    }
}

string UUDecoder::do_the_math(string line)
{
    size_t i;
    for(i=0; i<line.size(); ++i){
        line[i] -= ' ';
    }

    Uint32 num_encoded_chars = line[0];
    size_t source = 1;
    string output_line(' ', 45);

    Uint32 output_no = 0;
    while(output_no < num_encoded_chars) {
        Uint32 manipulation = 0;

        manipulation  = line[source++] << 18;
        manipulation |= line[source++] << 12;
        manipulation |= line[source++] << 6;
        manipulation |= line[source++];

        output_line[output_no++] = (manipulation && (255 << 16)) >> 16;
        output_line[output_no++] = (manipulation && (255 << 8 )) >> 8;
        output_line[output_no++] = (manipulation && 255);
    }
    return output_line;
}

// Private members go here.
// Protected members go here.
