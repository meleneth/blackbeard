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
        Uint32 min_article_no(void);
        Uint32 max_article_no(void);
        string status(void);
        void expire(void);
        Uint32 file_index(PostFile *file);
        Uint32 index();
        Uint32 num_files();
        unsigned long long int num_bytes();
        void needs_full_info();
        static bool compare(const PostSet* a, const PostSet* b);
        bool operator< (const PostSet &b);

        void restore_saved_info();
        void save_info();

        string info_filename(void);

        string subject;
        string poster;

        vector<PostFile *> files;
        string pattern_name;

        NewsGroup *group;
        Uint32 db_index;

        Uint32 has_pieces_loaded;
        Uint32 _num_files;
        Uint64 _num_bytes;
        Uint32 _min_article_no;
        Uint32 _max_article_no;
        PostFile *_last_file;
        Uint32 tick;
        Uint32 date;

        POSTSET_STATUS _status;
        ParArchive *par;
    private:
    protected:
};

extern vector<PostSet *> downloaded_postsets;

#endif
