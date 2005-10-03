#ifndef ___messageheader_inc
#define ___messageheader_inc 1

#include "SDL_types.h"
#include "newsgroup.hpp"
#include <string>

using std::string;

class NewsGroup;
class MessageHeader {
    public:
        MessageHeader(NewsGroup *group, Uint32 message_id, string subject, string posted_by);
        ~MessageHeader();

        NewsGroup *group;
        Uint32 message_id;
        string subject;
        string posted_by;
        Uint32 process_count;
    private:
    protected:
};

#endif
