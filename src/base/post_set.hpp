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
        Uint32 min_msg_id(void);
        Uint32 max_msg_id(void);
        string status(void);
        void recalculate_piece_info();

        string subject;
        vector<PostFile *> files;
        string pattern_name;

        Uint32 num_files;
        Uint32 max_num_files;
        Uint32 num_pieces;
        Uint32 num_finished_pieces;
        Uint32 has_msg_ids;
        Uint32 _min_msg_id;
        Uint32 _max_msg_id;
    private:
    protected:
};

void lock_postsets(void);
void unlock_postsets(void);

#endif
