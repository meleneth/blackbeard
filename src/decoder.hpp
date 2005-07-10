#ifndef ___decoder_inc
#define ___decoder_inc 1

#include"SDL_types.h"
#include<string>
#include"newsgrouppost.hpp"
#include"news_constants.hpp"
#include"stringpattern.hpp"
#include"post_file.hpp"

using std::string;

class PostFile;
class Decoder : public Job {
    public:
        // Public data members go here.
        Decoder(NewsGroupPost *newsgrouppost, PostFile *file); // Constructor
        Decoder();
        virtual ~Decoder(); // Destructor

        virtual void decode();
        virtual void decode_line(string line);
        virtual void open_file();
        virtual void close_file();

        string filename;
        FILE *fileptr;

        PostFile *post_file;
        NewsGroupPost *post;
        Uint32 piece_no;
        Uint32 num_bytes_written;

        enum DECODER_STATUS status;

        StringPattern *header_pattern;
        StringPattern *part_pattern;
        StringPattern *footer_pattern;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
