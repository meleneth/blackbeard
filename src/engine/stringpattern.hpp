#ifndef ___stringpattern_inc
#define ___stringpattern_inc 1

#include"SDL_types.h"
#include"news_constants.hpp"
#include<string>
#include<list>
#include<vector>

using std::list;
using std::vector;
using std::string;

class StringPattern {
    public:
        // Public data members go here.
        StringPattern();
        ~StringPattern(); 
        
        void add_breaker(string pattern);
        void add_breaker(SubjectPart pattern);
        int does_match(string target);
        void pieces(string target, vector<string>& result);

        list<string> breakers;
        vector<SubjectPart> part_order;

        vector<string> results;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
