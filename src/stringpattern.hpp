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

        int match(string target);

        string get_piece(Uint32 index);
        Uint32 get_piecen(Uint32 index);

        list<string> breakers;
        vector<Uint32> part_order;
        Uint32 max_part_no;

        vector<string> results;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
