#ifndef ___newsgroup_inc
#define ___newsgroup_inc 1

#include"SDL.h"

class NewsGroup {
    public:
        // Public data members go here.
        NewsGroup(int bar); // Constructor
        ~NewsGroup(); // Destructor
        int Foo(void);

        std::string name;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
