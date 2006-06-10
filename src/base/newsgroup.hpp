#ifndef ___newsgroup_inc
#define ___newsgroup_inc 1

#include"newsgrouppost.hpp"
#include"post_set.hpp"
#include"post_set_splitter.hpp"
#include"post_file.hpp"
#include"stringpattern.hpp"

#include "SDL_types.h"
#include <string>
#include <list>
#include <vector>
#include <sqlite3.h>

using std::list;
using std::vector;
using std::string;

class PostFile;
class PostSet;
class PostSetSplitter;
class NewsGroupPost;
class NewsGroup {
    public:
        // Public data members go here.
        NewsGroup(string group_name); // Constructor
        ~NewsGroup(); // Destructor
        void header_scoop(string xover_line);
        Uint32 index();
        static bool compare(const NewsGroup *a, const NewsGroup *b);
        void expire_old_postsets(Uint32 low_article_no);
        Uint32 postset_index(PostSet *set);
        Uint32 highest_seen_article_no();
        void subscribe(void);
        void unsubscribe(void);

        PostSet *postset_for_subject(string subject);
        
        int status_code();
        string status_string();

        list< NewsGroupPost * > messages;
        vector< PostSet * > postsets;
        string status;
        string name;
        Uint32 is_subscribed;
        Uint32 first_article_number;
        Uint32 last_article_number;
        PostSetSplitter *splitter;
        Uint32 db_index;
    private:
        int return_code;

        long current_article_number;
        long number_of_articles;
        std::string last_group_name;

    protected:
};


NewsGroup *group_for_name(string groupname);

extern vector<NewsGroup *> newsgroups;

#endif
