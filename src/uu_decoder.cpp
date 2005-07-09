#include "uu_decoder.hpp"

#include<sstream>
using std::stringstream;
#include"globals.hpp"
#include"strutil.hpp"

/* piece files are 7500 lines
 * lines hold 45 bytes each (decoded)
 * which comes out to 337,500 bytes encoded per uuencoded piece file
 */

#define UUDECODER_OFFSET 337500

UUDecoder::UUDecoder()
{
}

UUDecoder::UUDecoder(NewsGroupPost *newsgrouppost, PostFile *file, string message_id) : Decoder(newsgrouppost, file)// Constructor
{
    status = S_MESSAGE;
    piece_no = file->piece_no(message_id);

    if(piece_no != 1){
        status = S_BODY;
        open_file();
    }

    filename = post_file->filename;
    header_pattern = new StringPattern(4);
    header_pattern->add_breaker("begin ");
    header_pattern->add_breaker(" ");
    num_encoded_chars = 0;
}

UUDecoder::~UUDecoder()
{
    post_file->piece_size = num_encoded_chars;
}

void UUDecoder::open_file(void)
{
    Decoder::open_file();

    size_t seek_offset =  post_file->piece_size * (piece_no - 1);

    fseek(fileptr, seek_offset, SEEK_SET);
}

void UUDecoder::decode_line(string line)
{
    if(S_MESSAGE == status){
        if(header_pattern->does_match(line)){
            header_pattern->pieces(line);
            filename = header_pattern->results[2];
            post_file->filename = filename;
            open_file();
            status = S_BODY;
        }
    }else if(S_BODY == status){
        if(line.compare("'") == 0)
            return;
        if(line.compare("end") == 0)
            return;
        string decoded_line = do_the_math(line);
        fwrite(decoded_line.c_str(), 1, decoded_line.length(), fileptr);
        num_encoded_chars += decoded_line.length();
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
    string output_line(num_encoded_chars, ' ');

    Uint32 output_no = 0;
    while(output_no < num_encoded_chars) {
        Uint32 manipulation = 0;

        manipulation  = line[source++] << 18;
        manipulation |= line[source++] << 12;
        manipulation |= line[source++] << 6;
        manipulation |= line[source++];
        output_line[output_no++] = (manipulation & (255 << 16)) >> 16;
        output_line[output_no++] = (manipulation & (255 << 8 )) >> 8;
        output_line[output_no++] = (manipulation & 255);
    }
    return output_line.substr(0, num_encoded_chars);
}

