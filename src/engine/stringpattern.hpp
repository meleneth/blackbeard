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
        StringPattern(Uint32 max_part_no);
        ~StringPattern(); 
        
        void add_breaker(string pattern);
        void add_breaker(Uint32 part_no);

        int does_match(string target);
        void pieces(string target);

        list<string> breakers;
        vector<Uint32> part_order;

        vector<string> results;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
