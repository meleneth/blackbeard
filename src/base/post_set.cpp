#include "post_set.hpp"
#include"console.hpp"
#include"config.hpp"
#include"strutil.hpp"
#include"mnzb.hpp"

#include<sstream>
#include<iomanip>
using std::stringstream;
using std::setprecision;

vector<PostSet *> downloaded_postsets;

PostSet::PostSet(string subject)
{
    this->subject = subject;
    pattern_name="";
    _min_article_no = 0;
    _max_article_no = 0;
    _num_bytes = 0;
    _num_files = 0;
    has_pieces_loaded = 0;
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
    needs_full_info();

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
    return has_pieces_loaded ? files.size() : _num_files;
}

string PostSet::status(void)
{
    stringstream buf;
    buf << subject << ", " << num_files() << " files. " << setprecision(3) << completed_percent() << "%";
    return buf.str();
}


Uint32 PostSet::max_article_no(void)
{
    if(!has_pieces_loaded){
        return _max_article_no;
    }

    Uint32 article_no = 0;

    Uint32 max = files.size();
    for(Uint32 i=0; i<max; ++i) {
        if(files[i]){
            Uint32 x = files[i]->max_article_no();
            if(article_no < x)
                article_no = x;
        }
    }

    return article_no;
}

Uint32 PostSet::min_article_no(void)
{
    if(!has_pieces_loaded){
        return _min_article_no;
    }

    Uint32 article_no = 0;
    article_no--;

    Uint32 max = files.size();
    for(Uint32 i=0; i<max; ++i) {
        if(files[i]){
            Uint32 x = files[i]->min_article_no();
            if(x){
                if(article_no > x)
                    article_no = x;
            }
        }
    }
    return article_no;
}

unsigned long long int PostSet::num_bytes(void)
{
    if(!has_pieces_loaded){
        return _num_bytes;
    }
    
    _num_bytes = 0;

    Uint32 max = files.size();
    for(Uint32 i=0; i<max; ++i) {
        if(files[i]){
            _num_bytes += files[i]->num_bytes();
        }
    }
    return _num_bytes;
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

string PostSet::info_filename(void)
{
    console->log(get_crc_32(subject));
    return get_crc_32(subject);
}

void PostSet::needs_full_info()
{
    if(!has_pieces_loaded){
        has_pieces_loaded = 1;
        console->log("Loading full info for " + subject);
        mNZB nzb;
        nzb.load_postset(this);
    }
}

