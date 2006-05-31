#include "messageheader.hpp"

MessageHeader::MessageHeader(NewsGroup *group, Uint32 article_no, string msg_id, string subject, string posted_by, Uint32 num_bytes)
{
    this->group = group;
    this->subject = subject;
    this->posted_by = posted_by;
    this->msg_id = msg_id;
    this->article_no = article_no;
    this->num_bytes = num_bytes;
    process_count = 0;
}

MessageHeader::~MessageHeader()
{
}
