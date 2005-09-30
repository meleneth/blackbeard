#include "messageheader.hpp"

MessageHeader::MessageHeader(NewsGroup *group, Uint32 message_id, string subject, string posted_by)
{
    this->group = group;
    this->message_id = message_id;
    this->subject = subject;
    this->posted_by = posted_by;
}

MessageHeader::~MessageHeader()
{
}
