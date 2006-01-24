#include "filepiece.hpp"

FilePiece::FilePiece(Uint32 msg_id, PIECE_STATUS status, PostFile *file)
{
    this->msg_id = msg_id;
    this->status = status;
    this->file = file;
}

FilePiece::~FilePiece()
{
}

