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
    status = "-ignored-";
    piece_size = 0;
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
        stringstream buf;
        buf << "Unhandled error: num pieces changed on message.  Was "
            << max_part_no << " and is now " << num_pieces << " <" << filename << ">";
        console->log(buf.str());
        return;
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
            return new yEncDecoder(newsgrouppost, this);
        case DT_UUDECODE:
            return new UUDecoder(newsgrouppost, this, message_id);
        case DT_MIME:
        case DT_UNKNOWN:
            return NULL;
    }
    return NULL;
}

string PostFile::status_string(void)
{
    stringstream mystatus;
    mystatus << filename << " - " << status << " - ";
    mystatus << seen_pieces << "/" << num_pieces << " pieces seen   " 
             << downloaded_pieces << "/"  << num_pieces << " pieces downloaded  ";
    if(num_pieces == downloaded_pieces){
        mystatus << "100%";
    }else{
        if(num_pieces > 0)
           mystatus << setprecision(3) 
                  << ((double)downloaded_pieces / (double)num_pieces) * (double) 100
                  << "%";
    }
    return mystatus.str();
}

string PostFile::get_bar(void)
{
    string bar(24, ' ');
    const char throbber[4] = {'.', 'o', 'O', 'o'};

    if(num_pieces == downloaded_pieces) 
        return "Completed";
    if(num_pieces > 0) {
        int spaces = (int)floor(((double)downloaded_pieces / (double)num_pieces) * (double)20);
        bar[spaces+3] = '>' ;
    }
    if(status.compare("Downloading") == 0){
        bar[0] = throbber[downloaded_pieces % 4];
    }
    bar[2] = '[';
    bar[23] = ']';
    return bar;
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