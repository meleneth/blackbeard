#include "uu_decoder.hpp"

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

    fwrite(output_line.c_str(), 1, output_no, fileptr);
}

// Private members go here.
// Protected members go here.
