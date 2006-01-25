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
    pattern_name="";
    _min_msg_id = 0;
    _max_msg_id = 0;
    has_msg_ids = 0;
    group = NULL;
    _last_file = NULL;
    tick = 1;
    par = new ParArchive();
    db_index = 0;
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
            total_pieces += (*v)->pieces.size();
            downloaded_pieces += (*v)->num_downloaded_pieces();
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
    return postfile;
}

Uint32 PostSet::num_files(void)
{
    return files.size();
}

string PostSet::status(void)
{
    stringstream buf;
    buf << subject << ", " << num_files() << " files. " << setprecision(3) << completed_percent() << "%";
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


void PostSet::expire(void)
{
}
