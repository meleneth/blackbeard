#include "post_set.hpp"
#include"console.hpp"
#include"config.hpp"

#include<sstream>
#include<iomanip>
using std::stringstream;
using std::setprecision;

vector<PostSet *> downloaded_postsets;

PostSet::PostSet(string subject)
{
    this->subject = subject;
    num_files = 0;
    num_pieces = 0;
    num_finished_pieces = 0;
    pattern_name="";
    _max_num_files = 0;
    _min_msg_id = 0;
    _max_msg_id = 0;
    has_msg_ids = 0;
    group = NULL;
    _last_file = NULL;
    tick = 1;
    par = new ParArchive();
}

PostSet::~PostSet()
{
    Uint32 max_no = files.size();
    for(Uint32 i=0; i<max_no; ++i){
        delete files[i];
    }
    files.clear();
}

double PostSet::completed_percent(void)
{
    Uint32 total_pieces = 0;
    Uint32 downloaded_pieces = 0;
    if(files.begin() == files.end())
        return 0;

    vector<PostFile *>::iterator v;

    for (v = files.begin(); v != files.end(); ++v){
        if(*v){
            total_pieces += (*v)->num_pieces;
            downloaded_pieces += (*v)->downloaded_pieces;
        }
    }

    if(total_pieces == downloaded_pieces)
        return 100;

    return ((double)downloaded_pieces / (double)total_pieces * (double) 100);
}

PostFile *PostSet::file(string filename)
{
    tick = config->tick;

    if(_last_file){
        if(_last_file->filename.compare(filename) == 0){
            return _last_file;
        }
    }
    vector<PostFile *>::iterator f;
    for(f = files.begin(); f!= files.end() ; ++f){
        if(*f){
            if((*f)->filename.compare(filename) == 0){
                (*f)->tick = tick;
                _last_file = *f;
                return *f;
            }
        }
    }

    PostFile *postfile = new PostFile(this);
    postfile->filename = filename;
    postfile->tick = tick;
    files.push_back(postfile);
    num_files++;
    _max_num_files = files.size();
    return postfile;
}

void PostSet::recalculate_piece_info()
{
    Uint32 tnum_pieces = 0;
    Uint32 tnum_finished_pieces = 0;
    Uint32 tnum_files = 0;

    Uint32 max_size = files.size();
    for(Uint32 i = 0; i < max_size; ++i){
        PostFile *file = files[i];
        if(file){
            tnum_files++;
            tnum_pieces += file->num_pieces;
            tnum_finished_pieces += file->downloaded_pieces;
        }
    }
    num_pieces = tnum_pieces;
    num_finished_pieces = tnum_finished_pieces;
    num_files = tnum_files;
    _max_num_files = files.size();
}

Uint32 PostSet::max_num_files(void)
{
    if(has_msg_ids){
        recalculate_piece_info();
    }
    return _max_num_files;
}

string PostSet::status(void)
{
    stringstream buf;
    buf << subject << ", " << num_files << " files. " << setprecision(3) << completed_percent() << "%";
    return buf.str();
}


Uint32 PostSet::max_msg_id(void)
{
    if(!has_msg_ids){
        return _max_msg_id;
    }

    Uint32 msg_id = 0;

    Uint32 max = files.size();
    for(Uint32 i=0; i<max; ++i) {
        if(files[i]){
            Uint32 x = files[i]->max_msg_id();
            if(msg_id < x)
                msg_id = x;
        }
    }

    return msg_id;
}

Uint32 PostSet::min_msg_id(void)
{
    if(!has_msg_ids){
        return _min_msg_id;
    }

    Uint32 msg_id = 0;
    msg_id--;

    Uint32 max = files.size();
    for(Uint32 i=0; i<max; ++i) {
        if(files[i]){
            Uint32 x = files[i]->min_msg_id();
            if(x){
                if(msg_id > x)
                    msg_id = x;
            }
        }
    }
    return msg_id;
}

bool PostSet::compare(const PostSet* a, const PostSet* b)
{
    return (a->subject < b->subject);
}

Uint32 PostSet::file_index(PostFile *file)
{
    Uint32 max_no = files.size();
    for(Uint32 i=0; i<max_no; ++i) {
        if(file == files[i]) {
            return i;
        }
    }
    return 0;
}

Uint32 PostSet::index()
{
    Uint32 max_no = group->postsets.size();
    for(Uint32 i=0; i<max_no; ++i) {
        if(group->postsets[i] == this){
            return i;
        }
    }
    exit(1);
}

void PostSet::save_postfiles(sqlite3 *db, Uint32 postset_no)
{
    sqlite3_stmt *pf;
    string pf_stmt = "INSERT INTO post_files VALUES(?, ?, ?)";
    sqlite3_prepare(db, pf_stmt.c_str(), pf_stmt.length(), &pf, 0);
    Uint32 max_no = files.size();
    for(Uint32 i=0; i<max_no; i++){
        PostFile *file = files[i];
        sqlite3_bind_int(pf, 1, i); 
        sqlite3_bind_int(pf, 2, postset_no); 
        sqlite3_bind_text(pf, 3, file->filename.c_str(), file->filename.length(), NULL);
        sqlite3_step(pf);
        sqlite3_reset(pf);
        file->save_ids_to_db(db, i);
    }
    sqlite3_finalize(pf);
}
