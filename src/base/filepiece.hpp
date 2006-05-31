#ifndef ___filepiece_inc
#define ___filepiece_inc 1

#include "SDL_types.h"

typedef enum { MISSING, SEEN, DOWNLOADING, DECODING, FINISHED } PIECE_STATUS;

#include "post_file.hpp"

class PostFile;
class FilePiece {
    public:
        FilePiece(Uint32 article_no, string msg_id, PIECE_STATUS status, PostFile *file);
        ~FilePiece();

        void change_status(PIECE_STATUS new_status);

        Uint32 article_no;
        string msg_id;
        
        PIECE_STATUS status;
        PostFile *file;
        Uint32 _status_changed;
        Uint32 db_index;
    private:
    protected:
};

#endif
