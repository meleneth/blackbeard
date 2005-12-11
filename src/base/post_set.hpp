#ifndef ___post_set_inc
#define ___post_set_inc 1

#include"post_file.hpp"
#include"job.hpp"
#include"newsgroup.hpp"

#include"SDL_types.h"
#include<string>
#include<vector>

using std::string;
using std::vector;

class PostFile;
class NewsGroup;
class PostSet {
    public:
        PostSet(string subject);
        ~PostSet(); 
    
        double completed_percent(void);
        PostFile *file(string file_name);
        Uint32 min_msg_id(void);
        Uint32 max_msg_id(void);
        string status(void);
        void recalculate_piece_info();
        Uint32 max_num_files(void);
        Uint32 file_index(PostFile *file);
        static bool compare(const PostSet* a, const PostSet* b);
        bool operator< (const PostSet &b);

        string subject;
        vector<PostFile *> files;
        string pattern_name;

        NewsGroup *group;

        Uint32 num_files;
        Uint32 _max_num_files;
        Uint32 num_pieces;
        Uint32 num_finished_pieces;
        Uint32 has_msg_ids;
        Uint32 _min_msg_id;
        Uint32 _max_msg_id;
        PostFile *_last_file;
        Uint32 tick;
    private:
    protected:
};

extern vector<PostSet *> downloaded_postsets;

#endif
