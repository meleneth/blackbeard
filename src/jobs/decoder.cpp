#include "decoder.hpp"
#include "console.hpp"
#include "config.hpp"
#include "file_handle.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>  // I/O 
#include <fstream>   // file I/O


#define DECODER_LINES_PER_SLICE 200

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
    file = post_file->open_file();
    file_is_open = 1;
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
