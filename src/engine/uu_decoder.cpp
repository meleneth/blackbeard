#include "uu_decoder.hpp"

/* piece files are 7500 lines
 * lines hold 45 bytes each (decoded)
 * which comes out to 337,500 bytes encoded per uuencoded piece file
 */

#define UUDECODER_OFFSET 337500

// Public data members go here.
UUDecoder::UUDecoder(NewsGroupPost *newsgrouppost, Uint32 piece_no) : Decoder()// Constructor
{
    this->piece_no = piece_no;
    this->post = newsgrouppost;
}
    
UUDecoder::~UUDecoder() // Destructor
{
}

void UUDecoder::open_file(void)
{
    Decoder::open_file();

    fseek(fileptr, UUDECODER_OFFSET * (piece_no - 1), SEEK_SET);
}

void UUDecoder::decode_line(string line)
{
    size_t i;
    for(i=0; i<line.size(); ++i){
        line[i] -= ' ';
    }

    Uint32 num_encoded_chars = line[0];
    size_t source = 1;
    Uint32 manipulation;

    manipulation  = line[source++] << 24;
    manipulation |= line[source++] << 16;
    manipulation |= line[source++] << 8;
    manipulation |= line[source++];

    string output_line(' ', 45);
}

// Private members go here.
// Protected members go here.
