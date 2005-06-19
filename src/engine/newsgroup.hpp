#ifndef ___newsgroup_inc
#define ___newsgroup_inc 1

#include"newsgrouppost.hpp"

#include"SDL_types.h"
#include<string>
#include<list>

class NewsGroup {
    public:
        // Public data members go here.
        NewsGroup(std::string group_name); // Constructor
        ~NewsGroup(); // Destructor
        void header_scoop(std::string xover_line);
        
        int status_code();

        std::list< NewsGroupPost * > messages;
        std::string name;

    private:
        int return_code;

        long first_article_number;
        long last_article_number;
        long current_article_number;
        long number_of_articles;
        std::string last_group_name;

    protected:
        // Protected members go here.
};

#endif
