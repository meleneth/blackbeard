#include "post_file.hpp"
#include "stringpattern.hpp"
#include"yenc_decoder.hpp"
#include"uu_decoder.hpp"
#include"console.hpp"
#include<math.h>
#include<config.hpp>

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
    status = "Ignored";
    piece_size = 0;
}
    
PostFile::~PostFile() 
{
}

Uint32 PostFile::is_par()
{
    StringPattern *pattern = new StringPattern(2);

    pattern->add_breaker(".PAR2");
    if(pattern->match(filename)){
        delete pattern;
        return 1;
    }
    delete pattern;
    pattern = new StringPattern(2);

    pattern->add_breaker(".par2");
    if(pattern->match(filename)){
        delete pattern;
        return 1;
    }
    delete pattern;
    return 0;
}

void PostFile::part(Uint32 part_no, Uint32 max_part_no, Uint32 message_id)
{
    if(num_pieces == 0){
        num_pieces = max_part_no;
        pieces.resize(num_pieces + 1);
        piece_status.resize(num_pieces + 1);

        pieces[max_part_no] = 0;
        piece_status[max_part_no] = MISSING;
        while(max_part_no){
            max_part_no--;
            pieces[max_part_no] = 0;
            piece_status[max_part_no] = MISSING;
        }
        max_part_no = num_pieces;
    }

    if(max_part_no != num_pieces){
        if(config->debug_logging){
            stringstream buf;
            buf << "Unhandled error: num pieces changed on message.  Was "
                << max_part_no << " and is now " << num_pieces << " <" << filename << ">";
            console->log(buf.str());
        }
        return;
    }

    if(part_no > max_part_no){
        // scoop scoop
        if(config->debug_logging)
            console->log("Not adding nonsensicle part no");

    }else{
        pieces[part_no] = message_id;
        piece_status[part_no] = SEEN;
        seen_pieces++;
        if(num_pieces < seen_pieces){
            num_pieces = seen_pieces;
        }
    }
}

string PostFile::status_string(void)
{
    stringstream mystatus;
    mystatus << filename << " - " << status << " - ";
    mystatus << downloaded_pieces << "/"  << num_pieces << " pieces downloaded  ";
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

Uint32 PostFile::piece_no(Uint32 message_id)
{
    Uint32 i=0;
    vector<Uint32>::iterator p;
    for(p = pieces.begin() ; p != pieces.end() ; ++p){
        if(*p == message_id){
            return i;
        }
        i++;
    }
    return 0;
}

Uint32 PostFile::min_msg_id(void)
{
    Uint32 msg_id = 0;
    msg_id--;

    Uint32 max = pieces.size();
    for(Uint32 i=0; i<max; ++i) {
        Uint32 x = pieces[i];
        if(x){
            if(msg_id > x)
                msg_id = x;
        }
    }
    return msg_id;
}

Uint32 PostFile::max_msg_id(void)
{
    Uint32 msg_id = 0;

    Uint32 max = pieces.size();
    for(Uint32 i=0; i<max; ++i) {
        Uint32 x = pieces[i];
        if(msg_id < x)
            msg_id = x;
    }

    return msg_id;
}

void PostFile::saw_message_id(Uint32 msg_id)
{
    vector<Uint32>::iterator p;
    for(p = pieces.begin() ; p != pieces.end() ; ++p){
        if(*p == msg_id){
            return;
        }
    }
    seen_pieces++;
    if(num_pieces < seen_pieces){
        num_pieces = seen_pieces;
    }
    pieces.push_back(msg_id);
    piece_status.push_back(SEEN);
}

bool PostFile::compare(const PostFile* a, const PostFile* b)
{
    return (a->filename < b->filename);
}

void PostFile::update_status_from_pieces(void)
{
    status = "Ignored";
    Uint32 max_no = pieces.size();
    Uint32 finished_count = 0;
    Uint32 seen_count = 0;
    Uint32 downloading_count = 0;

    for(Uint32 i=0; i<max_no; ++i) {
        switch(piece_status[i]){
            case MISSING:
                status = "Missing Pieces";
                return;
                break;
            case SEEN:
                seen_count++;
                break;
            case DOWNLOADING:
                status = "Downloading";
                downloading_count++;
                break;
            case DECODING:
                break;
            case FINISHED: 
                finished_count++;
                break;
        }
    }

    if(num_pieces < max_no)
        num_pieces = max_no;

    if(finished_count == num_pieces)
        status = "Finished";

    downloaded_pieces = downloading_count;
}
