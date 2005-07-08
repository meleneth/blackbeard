#include "post_file.hpp"
#include"globals.hpp"
#include"yenc_decoder.hpp"
#include"uu_decoder.hpp"
#include<math.h>

#include<sstream>
#include<iomanip>

using std::stringstream;
using std::setprecision;

PostFile::PostFile(PostSet *postset) 
{
    num_pieces = 0;
    seen_pieces = 0;
    downloaded_pieces = 0;
    post_set = postset;
    decoder_type = DT_UNKNOWN;
}
    
PostFile::~PostFile() 
{
}

void PostFile::part(Uint32 part_no, Uint32 max_part_no, string message_id)
{
    if(num_pieces == 0){
        num_pieces = max_part_no;
        pieces.resize(num_pieces + 1);

        pieces[max_part_no] = "";
        while(max_part_no){
            max_part_no--;
            pieces[max_part_no] = "";
        }
        max_part_no = num_pieces;
    }

    if(max_part_no != num_pieces){
        console->log("Unhandled error: num pieces changed on message");
    }

    if(part_no > max_part_no){
        console->log("Not adding nonsensicle part no");
    }else{
        pieces[part_no] = message_id;
        seen_pieces++;
    }
}

Decoder *PostFile::get_decoder(NewsGroupPost *newsgrouppost, string dest_dir, string message_id)
{
    switch(decoder_type){
        case DT_YENC:
            return new yEncDecoder(newsgrouppost, dest_dir + "/" + filename);
        case DT_UUDECODE:
            return new UUDecoder(newsgrouppost, this, message_id);
        case DT_MIME:
        case DT_UNKNOWN:
            return NULL;
    }
    return NULL;
}

string PostFile::status(void)
{
    stringstream status;
    status << filename << " - ";
    status << seen_pieces << "/" << num_pieces << " pieces seen   " 
           << downloaded_pieces << "/"  << num_pieces << " pieces downloaded  ";
    if(num_pieces == downloaded_pieces){
        status << "100%";
    }else{
        if(num_pieces > 0)
           status << setprecision(3) 
                  << ((double)downloaded_pieces / (double)num_pieces) * (double) 100
                  << "%";
    }
    return status.str();
}

string PostFile::get_bar(void)
{
    string bar(20, ' ');
    static int frame=0;
    const char throbber[4] = {'.', 'o', 'O', 'o'};

    if(num_pieces == downloaded_pieces) 
        return "Completed";
    if(num_pieces > 0) {
        int spaces = downloaded_pieces / num_pieces;
        bar[spaces] = '>' ;
    }
    if(++frame > 3) frame = 0;

    return throbber[frame] + " [" + bar + "]";
}

Uint32 PostFile::piece_no(string message_id)
{
    Uint32 i=0;
    vector<string>::iterator p;
    for(p = pieces.begin() ; p != pieces.end() ; ++p){
        if(!(*p).compare(message_id)){
            return i;
        }
        i++;
    }
    return 0;
}
