#include "filepiece.hpp"

FilePiece::FilePiece(Uint32 article_no, string msg_id, PIECE_STATUS status, PostFile *file)
{
    this->article_no = article_no;
    this->status = status;
    this->file = file;
    this->msg_id = msg_id;
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
