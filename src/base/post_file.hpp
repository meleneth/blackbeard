#ifndef ___post_file_inc
#define ___post_file_inc 1

#include<string>
#include<vector>
#include"SDL_types.h"
#include"post_set.hpp"
#include"decoder.hpp"
#include"news_constants.hpp"
#include"newsgrouppost.hpp"

using std::string;
using std::vector;

class PostSet;
class Decoder;
class PostFile {
    public:
        PostFile(PostSet *set);
        ~PostFile();

        void part(Uint32 part, Uint32 total_parts, Uint32 message_id);
        string status_string(void);
        string get_bar(void);
        Decoder *get_decoder(NewsGroupPost *newsgrouppost, string dest_dir, Uint32 message_id);
        Uint32 piece_no(Uint32 message_id);

        Uint32 num_pieces;
        Uint32 seen_pieces;
        Uint32 piece_size;
        Uint32 downloaded_pieces;
        string filename;
        string status;

        PostSet *post_set;

        DecoderType decoder_type;

        vector<Uint32> pieces;
    private:
    protected:
};

#endif
