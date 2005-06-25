#ifndef ___post_file_inc
#define ___post_file_inc 1

#include<string>
#include<vector>
#include"SDL_types.h"

using std::string;
using std::vector;

class PostFile {
    public:
        PostFile();
        ~PostFile(); 

        void part(Uint32 part, Uint32 total_parts, string message_id);
        string status(void);

        Uint32 num_pieces;
        Uint32 seen_pieces;
        string filename;

        vector<string> pieces;
    private:
    protected:
};

#endif
