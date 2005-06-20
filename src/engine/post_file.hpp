#ifndef ___post_file_inc
#define ___post_file_inc 1

#include<string>
#include"SDL_types.h"

using std::string;

class PostFile {
    public:
        PostFile();
        ~PostFile(); 

        void part(string part_no, string max_port_no);

        Sint32 num_pieces;
        
    private:
    protected:
};

#endif
