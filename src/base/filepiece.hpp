#ifndef ___filepiece_inc
#define ___filepiece_inc 1

#include "SDL_types.h"
#include "post_file.hpp"

typedef enum { MISSING, SEEN, DOWNLOADING, DECODING, FINISHED } PIECE_STATUS;

class PostFile;
class FilePiece {
    public:
        FilePiece(Uint32 msg_id, PIECE_STATUS status, PostFile *file);
        ~FilePiece();

        Uint32 msg_id;
        PIECE_STATUS status;
        PostFile *file;
    private:
    protected:
};

#endif
