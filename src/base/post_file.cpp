#include "post_file.hpp"
#include "stringpattern.hpp"
#include "yenc_decoder.hpp"
#include "uu_decoder.hpp"
#include "console.hpp"
#include "strutil.hpp"
#include "config.hpp"
#include "database.hpp"

#include <math.h>
#include <sstream>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using std::stringstream;
using std::setprecision;

PostFile::PostFile(PostSet *postset) 
{
    post_set = postset;
    decoder_type = DT_UUDECODE;
    status = "Ignored";
    piece_size = 0;
    is_corrupt = 0;
    has_db_pieces = 0;
    tick = 1;
    db_index = 0;
    _num_file_pieces = 0;
    _num_downloaded_pieces = 0;
}
    
PostFile::~PostFile() 
{
}

Uint32 PostFile::is_par()
{
    StringPattern pattern(2);

    pattern.add_breaker(".PAR2");
    if(pattern.match(filename)){
        return 1;
    }
    return 0;
}

string PostFile::status_string(void)
{
    stringstream mystatus;
    mystatus << filename << " - " << status << " - ";
    mystatus << num_downloaded_pieces() << "/"  << num_pieces() << " pieces downloaded  ";
    if(num_pieces() == num_downloaded_pieces()){
        mystatus << "100%";
   }else{
        if(num_pieces())
           mystatus << setprecision(3) 
                  << ((double)num_downloaded_pieces() / (double)num_pieces()) * (double) 100
                  << "%";
    }
    return mystatus.str();
}

string PostFile::get_bar(void)
{
    string bar(24, ' ');
    const char throbber[4] = {'.', 'o', 'O', 'o'};

    if(num_pieces() == num_downloaded_pieces())
        return "Completed";
    if(num_downloaded_pieces() > 0) {
        int spaces = (int)floor(((double)num_downloaded_pieces() / (double)pieces.size()) * (double)20);
        bar[spaces+3] = '>' ;
        bar[2] = '[';
        bar[23] = ']';
    }
    if(status.compare("Downloading") == 0){
        bar[0] = throbber[num_downloaded_pieces() % 4];
    }
    return bar;
}

Uint32 PostFile::piece_no(Uint32 message_id)
{
    Uint32 i=0;
    vector<FilePiece *>::iterator p;
    for(p = pieces.begin() ; p != pieces.end() ; ++p){
        if((*p)->msg_id == message_id){
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
        Uint32 x = pieces[i]->msg_id;
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
        Uint32 x = pieces[i]->msg_id;
        if(msg_id < x)
            msg_id = x;
    }

    return msg_id;
}

void PostFile::saw_message_id(Uint32 msg_id)
{
    if(!has_db_pieces){
        restore_ids_from_db(this);
    }
    vector<FilePiece *>::iterator p;
    for(p = pieces.begin() ; p != pieces.end() ; ++p){
        if((*p)->msg_id == msg_id){
            (*p)->status = SEEN;
            return;
        }
    }
    pieces.push_back(new FilePiece(msg_id, SEEN, this));
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
        switch(pieces[i]->status){
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

    if(finished_count == pieces.size())
        status = "Finished";
}

Uint32 PostFile::index()
{
    Uint32 max_no = post_set->files.size();
    for(Uint32 i=0; i<max_no; ++i) {
        if(post_set->files[i] == this){
            return i;
        }
    }
    exit(1);
}

FileHandle *PostFile::open_file()
{
    struct stat my_stats;
    string dest_dir = config->blackbeard_dir + "/" + safe_dirname(post_set->subject);
    if(stat(dest_dir.c_str(), &my_stats) == -1){
        console->log("Creating dir for decode");
#ifdef __WIN32__
        mkdir(dest_dir.c_str());
#else
        mkdir(dest_dir.c_str(), 01777);
#endif
    }else {
        console->log("download dir found");
    }

    string real_filename = dest_dir + "/" + filename;
    return open_filehandle(real_filename);
}

Uint32 PostFile::num_downloaded_pieces()
{
    return _num_downloaded_pieces;
}

Uint32 PostFile::count_num_downloaded_pieces()
{
    Uint32 num = 0;

    Uint32 max_no = pieces.size();
    for(Uint32 i=0; i<max_no; ++i) {

        if(pieces[i]->status == FINISHED){
            num++;
        }
    }
    return num;
}

Uint32 PostFile::num_pieces()
{
    if(has_db_pieces) {
        return pieces.size();
    }
    return _num_file_pieces;
}
