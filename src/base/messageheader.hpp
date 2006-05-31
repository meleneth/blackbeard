#ifndef ___messageheader_inc
#define ___messageheader_inc 1

#include "SDL_types.h"
#include "newsgroup.hpp"
#include <string>

using std::string;

class NewsGroup;
class MessageHeader {
    public:
        MessageHeader(NewsGroup *group, Uint32 article_no, string msg_id, string subject, string posted_by, Uint32 num_bytes);
        ~MessageHeader();

        NewsGroup *group;
        Uint32 article_no;
        string msg_id;
        string subject;
        string posted_by;
        Uint32 process_count;
        Uint32 num_bytes;
    private:
    protected:
};

#endif
