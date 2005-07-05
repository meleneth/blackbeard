#ifndef ___newsgroup_inc
#define ___newsgroup_inc 1

#include"newsgrouppost.hpp"
#include"post_set.hpp"
#include"stringpattern.hpp"

#include"SDL_types.h"
#include<string>
#include<list>

using std::list;
using std::string;

class NewsGroup {
    public:
        // Public data members go here.
        NewsGroup(string group_name); // Constructor
        ~NewsGroup(); // Destructor
        void header_scoop(string xover_line);
        void digest_subject_line(string message_id, string subject);

        PostSet *postset_for_subject(string subject);
        
        int status_code();

        list< StringPattern * > yenc_subject_patterns;
        list< NewsGroupPost * > messages;
        list< PostSet * > postsets;
        StringPattern *pattern;
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

#endif
