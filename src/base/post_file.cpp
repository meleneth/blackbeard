#include "post_file.hpp"
#include "stringpattern.hpp"
#include "yenc_decoder.hpp"
#include "uu_decoder.hpp"
#include "console.hpp"
#include "strutil.hpp"
#include "config.hpp"
#include "database.hpp"
#include "pararchive.hpp"

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
    status = "Ignored";
    is_corrupt = 0;
    tick = 1;
    db_index = 0;
    _num_downloaded_pieces = 0;
}
    
PostFile::~PostFile() 
{
}

string PostFile::status_string(void)
{
    stringstream mystatus;
    mystatus << human_readable_bytes(num_bytes()) << " " << filename << " - " << status << " - ";
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

void PostFile::log_info(void)
{
    stringstream buf;
    console->log("File info: " + filename);
    buf << pieces.size() << " pieces.";
    console->log(buf.str());
    buf.str("");
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
    list<FilePiece *>::iterator p;
    for(p = pieces.begin() ; p != pieces.end() ; ++p){
        if((*p)->article_no == message_id){
            return i;
        }
        i++;
    }
    return 0;
}

Uint32 PostFile::min_article_no(void)
{
    Uint32 article_no = 0;
    article_no--;

    list<FilePiece *>::iterator p;
    for(p = pieces.begin() ; p != pieces.end() ; ++p){
        Uint32 x = (*p)->article_no;
        if(x){
            if(article_no > x)
                article_no = x;
        }
    }
    return article_no;
}

Uint32 PostFile::max_article_no(void)
{
    Uint32 article_no = 0;

    list<FilePiece *>::iterator p;
    for(p = pieces.begin() ; p != pieces.end() ; ++p){
        Uint32 x = (*p)->article_no;
        if(article_no < x)
            article_no = x;
    }

    return article_no;
}

FilePiece *PostFile::saw_message_id(Uint32 article_no, string msg_id, Uint32 num_bytes)
{
    list<FilePiece *>::iterator p;
    for(p = pieces.begin() ; p != pieces.end() ; ++p){
        if((*p)->article_no == article_no){
            (*p)->status = SEEN;
            return *p;
        }
    }
    tick = config->tick;
    if(post_set)
        post_set->tick = config->tick;
    FilePiece *piece = new FilePiece(article_no, msg_id, SEEN, this, num_bytes);
    pieces.push_back(piece);
    return piece;
}

bool PostFile::compare(const PostFile* a, const PostFile* b)
{
    return (a->filename < b->filename);
}

void PostFile::switch_seen_statuses(PIECE_STATUS new_status)
{
    list<FilePiece *>::iterator p;
    for(p = pieces.begin() ; p != pieces.end() ; ++p){
        if((*p)->status != MISSING){
            (*p)->status = new_status;
            return;
        }
    }

}

void PostFile::update_status_from_pieces(void)
{
    status = "Ignored";
    Uint32 finished_count = 0;
    Uint32 seen_count = 0;
    Uint32 downloading_count = 0;

    list<FilePiece *>::iterator p;
    for(p = pieces.begin() ; p != pieces.end() ; ++p){
        switch((*p)->status){
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

void m_mkdir(string dir)
{
#ifdef __WIN32__
        mkdir(dir.c_str());
#else
        mkdir(dir.c_str(), 01777);
#endif
}

void ensure_directory_presence(string dirname, string logmsg) 
{
    struct stat my_stats;
    if(stat(dirname.c_str(), &my_stats) == -1){
        console->log(logmsg);
        m_mkdir(dirname);
    }
}

string PostFile::par_mangled_filename()
{
    return config->blackbeard_data_dir + "/" + post_set->group->name + "/par2/" + get_crc_32(post_set->subject) + ".par2"; 
}

FileHandle *PostFile::open_file()
{
    if(is_base_par(filename)) {
        if(!post_set)
            return NULL;
        string dirname = config->blackbeard_data_dir;
        ensure_directory_presence(dirname, "Creating blackbeard data dir..");
        dirname += "/" + post_set->group->name;
        ensure_directory_presence(dirname, "Creating blackbeard data dir for newsgroup..");
        dirname += "/par2";
        ensure_directory_presence(dirname, "Creating blackbeard par2 dir..");
        return open_filehandle(par_mangled_filename());
    }else {
        string dest_dir = config->blackbeard_dir + "/" + post_set->group->name;
        ensure_directory_presence(dest_dir, "Creating dir for newsgroup..");
        
        dest_dir += "/" + safe_dirname(post_set->subject) ;
        ensure_directory_presence(dest_dir, "Creating dir for decode");

        string real_filename = dest_dir + "/" + filename;
        return open_filehandle(real_filename);
    }
}



Uint32 PostFile::num_downloaded_pieces()
{
    return _num_downloaded_pieces;
}

Uint32 PostFile::count_num_downloaded_pieces()
{
    Uint32 num = 0;

    list<FilePiece *>::iterator p;
    for(p = pieces.begin() ; p != pieces.end() ; ++p){

        if((*p)->status == FINISHED){
            num++;
        }
    }
    return num;
}

Uint32 PostFile::num_pieces()
{
    return pieces.size();
}

Uint32 PostFile::num_bytes(void)
{
    Uint32 num = 0;

    list<FilePiece *>::iterator p;
    for(p = pieces.begin() ; p != pieces.end() ; ++p){
        num += (*p)->num_bytes;
    }
    return num;
}

Uint32 PostFile::num_downloaded_bytes(void)
{
    Uint32 num = 0;

    list<FilePiece *>::iterator p;
    for(p = pieces.begin() ; p != pieces.end() ; ++p){
        if((*p)->status == FINISHED){
            num += (*p)->num_bytes;
        }
    }
    return num;
}
