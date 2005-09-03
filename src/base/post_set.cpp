#include "post_set.hpp"
#include"console.hpp"
#include"config.hpp"

#include<sstream>
#include<iomanip>
using std::stringstream;
using std::setprecision;

PostSet::PostSet(string subject) 
{
    this->subject = subject;
    num_files = 0;
    num_pieces = 0;
    num_finished_pieces = 0;
    pattern_name="";
    max_num_files = 0;
    has_msg_ids = 0;
    group = NULL;
}
    
PostSet::~PostSet() 
{
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

PostFile *PostSet::file(Uint32 file_num, Uint32 max_file_num, string file_name)
{
    stringstream buf;

    //if(!file_name.compare("")){
    //   return NULL;
    //}

    if(max_file_num != 0) {
        if(!this->max_num_files){
            this->max_num_files = max_file_num;
            files.resize(max_num_files + 1);
            Uint32 i;
            for(i=0;i<max_num_files;i++){
                files[i] = NULL;
            }
        }

        if(file_num > max_num_files){
            if(config->debug_logging)
                console->log("Cowardly refusing to do a crashy op");
            return NULL;
        }

        if(files[file_num]){
            return files[file_num];
        }
        
        files[file_num] = new PostFile(this);
        files[file_num]->filename = file_name;
        num_files++;
        return files[file_num];
    }

    vector<PostFile *>::iterator f;
    for(f = files.begin(); f!= files.end() ; ++f){
        if(*f){
            if((*f)->filename.compare(file_name) == 0){
                return *f;
            }
        }
    }

    PostFile *postfile = new PostFile(this);
    postfile->filename = file_name;
    files.push_back(postfile);
    num_files++;
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
    max_num_files = files.size();
}

string PostSet::status(void)
{
    stringstream buf;
    buf << subject << ", " << num_files << " files. " << setprecision(3) << completed_percent() << "%";
    return buf.str();
}


Uint32 PostSet::max_msg_id(void)
{
    Uint32 msg_id = 0;
    msg_id--;

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
    Uint32 msg_id = 0;
    msg_id--;

    Uint32 max = files.size();
    for(Uint32 i=0; i<max; ++i) {
        if(files[i]){
            Uint32 x = files[i]->min_msg_id();
            if(msg_id > x)
                msg_id = x;
        }
    }
    return msg_id;
}

