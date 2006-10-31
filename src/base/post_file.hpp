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
#include "newsgroup.hpp"
#include "md5_stuff.hpp"

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
        FilePiece *saw_message_id(Uint32 article_no, string msg_id, Uint32 num_bytes);
        static bool compare(const PostFile* a, const PostFile* b);
        void update_status_from_pieces();
        Uint32 num_downloaded_pieces();
        Uint32 count_num_downloaded_pieces();

        void switch_seen_statuses(PIECE_STATUS new_status);
        void log_info();
        string par_mangled_filename();

        FileHandle *open_file();
        Uint32 index();
        Uint32 piece_no(Uint32 message_id);
        Uint32 max_article_no(void);
        Uint32 min_article_no(void);
        Uint32 num_pieces(void);
        Uint32 num_bytes(void);
        Uint32 num_downloaded_bytes(void);


        Uint32 is_corrupt;
        Uint32 tick;
        
        string filename;
        string status;

        PostSet *post_set;

        Uint32 db_index;
        Uint32 _num_downloaded_pieces;

        md5 hash;

        list<FilePiece *> pieces;
    private:
    protected:
};

void m_mkdir(string dirname);

#endif
