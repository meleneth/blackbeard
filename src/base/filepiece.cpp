#include "filepiece.hpp"

FilePiece::FilePiece(Uint32 msg_id, PIECE_STATUS status, PostFile *file)
{
    this->msg_id = msg_id;
    this->status = status;
    this->file = file;
    _status_changed = 0;
    db_index = 0;
}

FilePiece::~FilePiece()
{
}

void FilePiece::change_status(PIECE_STATUS new_status)
{
    status = new_status;
    _status_changed = 1;
}
