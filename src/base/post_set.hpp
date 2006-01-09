#ifndef ___post_set_inc
#define ___post_set_inc 1

#include "post_file.hpp"
#include "job.hpp"
#include "newsgroup.hpp"
#include "pararchive.hpp"

#include "SDL_types.h"
#include <string>
#include <vector>
#include <sqlite3.h>

using std::string;
using std::vector;

typedef enum { PS_VAGUE, PS_DOWNLOADING, PS_QUEUED, PS_KNOWN, PS_CORRUPT } POSTSET_STATUS;

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
        Uint32 index();
        static bool compare(const PostSet* a, const PostSet* b);
        bool operator< (const PostSet &b);

        void save_postfiles(sqlite3* db, Uint32 postset_no); 

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

        POSTSET_STATUS _status;
        ParArchive *par;
    private:
    protected:
};

extern vector<PostSet *> downloaded_postsets;

#endif
