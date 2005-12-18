#ifndef ___post_file_inc
#define ___post_file_inc 1

#include <string>
#include <vector>
#include "SDL_types.h"
#include "post_set.hpp"
#include "decoder.hpp"
#include "news_constants.hpp"
#include "newsgrouppost.hpp"
#include "file_handle.hpp"

using std::string;
using std::vector;

typedef enum { MISSING, SEEN, DOWNLOADING, DECODING, FINISHED } PIECE_STATUS;

class PostSet;
class Decoder;
class PostFile {
    public:
        PostFile(PostSet *set);
        ~PostFile();

        void part(Uint32 part, Uint32 total_parts, Uint32 message_id);
        string status_string(void);
        string get_bar(void);
        void saw_message_id(Uint32 msg_id);
        static bool compare(const PostFile* a, const PostFile* b);
        void update_status_from_pieces();
        FileHandle *open_file();
        Uint32 is_par();
        Uint32 piece_no(Uint32 message_id);
        Uint32 max_msg_id(void);
        Uint32 min_msg_id(void);

        Uint32 num_pieces;
        Uint32 seen_pieces;
        Uint32 piece_size;
        Uint32 downloaded_pieces;
        Uint32 is_corrupt;
        Uint32 tick;
        
        string filename;
        string status;

        PostSet *post_set;
        DecoderType decoder_type;

        vector<Uint32> pieces;
        vector<PIECE_STATUS> piece_status;
    private:
    protected:
};

#endif
