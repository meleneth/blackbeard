#ifndef ___decoder_inc
#define ___decoder_inc 1

#include "SDL_types.h"
#include <string>
#include "newsgrouppost.hpp"
#include "news_constants.hpp"
#include "stringpattern.hpp"
#include "post_file.hpp"
#include "job.hpp"
#include "file_handle.hpp"

using std::string;

class PostFile;
class Job;
class NewsGroupPost;

class Decoder : public Job {
    public:
        // Public data members go here.
        Decoder(NewsGroupPost *newsgrouppost, PostFile *file); // Constructor
        Decoder();
        virtual ~Decoder(); // Destructor

        virtual void process();
        virtual void decode_line(string line);
        virtual void open_file();
        virtual void close_file();
        virtual string status_line();

        string filename;

        PostFile *post_file;
        NewsGroupPost *post;
        Uint32 piece_no;
        Uint32 num_bytes_written;
        Uint32 file_is_open;

        DecoderStatus status;

        StringPattern *header_pattern;
        StringPattern *part_pattern;
        StringPattern *footer_pattern;
        FileHandle *file;
        Uint32 file_pos;

        Uint32 line_no;
        Uint32 max_line_no;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
