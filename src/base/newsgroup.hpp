#ifndef ___newsgroup_inc
#define ___newsgroup_inc 1

#include"newsgrouppost.hpp"
#include"post_set.hpp"
#include"post_file.hpp"
#include"stringpattern.hpp"

#include"SDL_types.h"
#include<string>
#include<list>
#include<vector>

using std::list;
using std::vector;
using std::string;

class PostFile;
class PostSet;
class NewsGroup {
    public:
        // Public data members go here.
        NewsGroup(string group_name); // Constructor
        ~NewsGroup(); // Destructor
        void header_scoop(string xover_line);
        PostFile *digest_subject_line(string message_id, string subject);
        void load_from_file(string filename);
        void save_postsets(void);
        void load_postsets(void);

        PostSet *postset_for_subject(string subject);
        
        int status_code();

        list< StringPattern * > yenc_subject_patterns;
        list< StringPattern * > uu_subject_patterns;
        list< NewsGroupPost * > messages;
        vector< PostSet * > postsets;
        string status;
        string name;
        Uint32 is_subscribed;
        Uint32 first_article_number;
        Uint32 last_article_number;
    private:
        int return_code;

        long current_article_number;
        long number_of_articles;
        std::string last_group_name;

    protected:
};

void load_groups_from(string filename);
void save_subscribed_groups_to(string filename);

NewsGroup *group_for_name(string groupname);

extern vector<NewsGroup *> newsgroups;

#endif
