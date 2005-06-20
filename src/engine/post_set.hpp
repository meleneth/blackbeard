#ifndef ___post_set_inc
#define ___post_set_inc 1

#include"post_file.hpp"

#include"SDL_types.h"
#include<string>
#include<vector>

using std::string;
using std::vector;

class PostSet {
    public:
        PostSet(string subject);
        ~PostSet(); 
    
        string completed_percent(void);
        PostFile *file(string file_num, string max_file_num, string file_name);
        string status(void);

        string subject;
        vector<PostFile *> files;

        string num_files;
        string num_pieces;
        string num_finished_pieces;
    private:
    protected:
};

#endif
