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
#include "filepiece.hpp"
#include <sqlite3.h>

using std::string;
using std::vector;


class PostSet;
class Decoder;
class FilePiece;
class PostFile {
    public:
        PostFile(PostSet *set);
        ~PostFile();

        string status_string(void);
        string get_bar(void);
        void saw_message_id(Uint32 msg_id);
        static bool compare(const PostFile* a, const PostFile* b);
        void update_status_from_pieces();
        Uint32 num_downloaded_pieces();

        FileHandle *open_file();
        Uint32 is_par();
        Uint32 index();
        Uint32 piece_no(Uint32 message_id);
        Uint32 max_msg_id(void);
        Uint32 min_msg_id(void);
        Uint32 num_pieces(void);


        Uint32 piece_size;
        Uint32 is_corrupt;
        Uint32 tick;
        
        string filename;
        string status;

        PostSet *post_set;
        DecoderType decoder_type;

        Uint32 db_index;
        Uint32 has_db_pieces;
        Uint32 _num_file_pieces;
        Uint32 _num_downloaded_pieces;

        vector<FilePiece *> pieces;
    private:
    protected:
};

#endif
