#ifndef ___newsgroup_inc
#define ___newsgroup_inc 1

#include"newsgrouppost.hpp"
#include"post_set.hpp"
#include"stringpattern.hpp"

#include"SDL_types.h"
#include<string>
#include<list>
#include<vector>

using std::list;
using std::vector;
using std::string;

class NewsGroup {
    public:
        // Public data members go here.
        NewsGroup(string group_name); // Constructor
        ~NewsGroup(); // Destructor
        void header_scoop(string xover_line);
        PostFile *digest_subject_line(string message_id, string subject);
        void load_from_file(string filename);

        PostSet *postset_for_subject(string subject);
        
        int status_code();

        list< StringPattern * > yenc_subject_patterns;
        list< StringPattern * > uu_subject_patterns;
        list< NewsGroupPost * > messages;
        vector< PostSet * > postsets;
        string status;
        string name;
    private:
        int return_code;

        long first_article_number;
        long last_article_number;
        long current_article_number;
        long number_of_articles;
        std::string last_group_name;

    protected:
};

void load_groups_from(string filename);
NewsGroup *group_for_name(string groupname);

#endif
