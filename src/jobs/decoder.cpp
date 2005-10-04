#include "decoder.hpp"
#include"console.hpp"
#include"config.hpp"

#include <sys/types.h>
#include <sys/stat.h>


#define DECODER_LINES_PER_SLICE 2000

// Public data members go here.
//
Decoder::Decoder()
{
    file_is_open = 0;
    file_pos = 0;
}

Decoder::Decoder(NewsGroupPost *newsgrouppost, PostFile *file) // Constructor
{
    post_file = file;
    post = newsgrouppost;
    piece_no = 0;
    filename = post_file->filename;
    status = S_MESSAGE;
    num_bytes_written = 0;
    file_is_open = 0;
}
    
Decoder::~Decoder() // Destructor
{
    if(file_is_open)
        close_file();
    close_finished_files();
}

void Decoder::process()
{
    list<string>::iterator s;
    Uint32 ctr = DECODER_LINES_PER_SLICE;
    Uint32 line_count = post->lines.size();

    if(!line_count){
        is_finished = 1;
    }

    while(line_count && ctr) {
        s = post->lines.begin();
        decode_line(*s);
        post->lines.erase(s);
        ctr--;
        line_count--;
    }
}

void Decoder::open_file(void)
{
    if(file_is_open)
        return;
    
    struct stat my_stats;
    string dest_dir = config->blackbeard_dir + "/" + safe_dirname(post_file->post_set->subject);
    if(stat(dest_dir.c_str(), &my_stats) == -1){
        mkdir(dest_dir.c_str(), 01777);
    }
    string real_filename = dest_dir + "/" + filename;
    file = new FileHandle(real_filename);
    file_is_open = 1;
}
string Decoder::safe_dirname(string unsafe)
{
    string s = unsafe;
    Uint32 max_no = s.length();
    Uint32 is_unsafe;
    for(Uint32 i=0; i<max_no; ++i){
        is_unsafe = 1;
        if(s[i] >= 'a' && s[i] <= 'z')
            is_unsafe = 0;
        if(s[i] >= 'A' && s[i] <= 'Z')
            is_unsafe = 0;
        if(is_unsafe)
            s[i] = '_';
    }
    return s;
}

void Decoder::close_file(void)
{
    if(!file_is_open)
        return;
    file->close();
    file_is_open = 0;
}

void Decoder::decode_line(string line)
{
}

// Private members go here.
// Protected members go here.
