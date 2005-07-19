#ifndef ___post_set_inc
#define ___post_set_inc 1

#include"post_file.hpp"

#include"SDL_types.h"
#include<string>
#include<vector>

using std::string;
using std::vector;

class PostFile;
class PostSet {
    public:
        PostSet(string subject);
        ~PostSet(); 
    
        double completed_percent(void);
        PostFile *file(Uint32 file_num, Uint32 max_file_num, string file_name);
        string status(void);
        void recalculate_piece_info();

        string subject;
        vector<PostFile *> files;
        string pattern_name;

        Uint32 num_files;
        Uint32 num_pieces;
        Uint32 num_finished_pieces;
    private:
    protected:
};

#endif