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
}

void UUDecoder::decode_line(string line)
{
}

// Private members go here.
// Protected members go here.
