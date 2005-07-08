#include "decoder.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include"globals.hpp"



// Public data members go here.
Decoder::Decoder(NewsGroupPost *newsgrouppost, PostFile *file) // Constructor
{
    post_file = file;
    post = newsgrouppost;
    piece_no = 0;
    filename = post_file->filename;
    status = S_MESSAGE;
    num_bytes_written = 0;
}
    
Decoder::~Decoder() // Destructor
{
    if(post){
        delete post;
    }
    close_file();
}

void Decoder::decode()
{
    list<string>::iterator s;
    
    for (s=post->lines.begin(); s!=post->lines.end(); ++s){
        decode_line(*s);
    }
}

void Decoder::open_file(void)
{
    struct stat my_stats;
    string dest_dir = config->blackbeard_dir + "/" + post_file->post_set->subject;
    if(stat(dest_dir.c_str(), &my_stats) == -1){
        mkdir(dest_dir.c_str(), 01777);
    }
    string real_filename = dest_dir + "/" + filename;
    console->log("Opening [" + real_filename + "] for writing");
    if(stat(real_filename.c_str(), &my_stats) == -1){
        fileptr = fopen(real_filename.c_str(), "w");
    } else {
        fileptr = fopen(real_filename.c_str(), "r+");
    }
}

void Decoder::close_file(void)
{
    fclose(fileptr);
}

void Decoder::decode_line(string line)
{
}

// Private members go here.
// Protected members go here.
