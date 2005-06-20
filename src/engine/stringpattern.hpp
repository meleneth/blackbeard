#ifndef ___stringpattern_inc
#define ___stringpattern_inc 1

#include"SDL_types.h"
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
        int does_match(string target);
        void pieces(string target, vector<string>& result);

        list<string> breakers;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
