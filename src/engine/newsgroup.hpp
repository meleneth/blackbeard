#ifndef ___newsgroup_inc
#define ___newsgroup_inc 1

#include"newsgrouppost.hpp"

#include"SDL.h"
#include<string>
#include<list>

class NewsGroup {
    public:
        // Public data members go here.
        NewsGroup(int bar); // Constructor
        ~NewsGroup(); // Destructor
        int Foo(void);

        std::string name;
        std::list< NewsGroupPost * > messages;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
