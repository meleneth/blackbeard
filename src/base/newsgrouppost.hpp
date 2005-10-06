#ifndef ___newsgrouppost_inc
#define ___newsgrouppost_inc 1

#include"SDL_types.h"

#include<string>
#include<list>

using std::list;
using std::string;

class NewsGroupPost {
    public:
        // Public data members go here.
        NewsGroupPost(); // Constructor
        ~NewsGroupPost(); // Destructor
        static bool compare(const NewsGroupPost *a, const NewsGroupPost *b);

        list<string> lines;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
