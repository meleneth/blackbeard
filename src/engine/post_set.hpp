#ifndef ___post_set_inc
#define ___post_set_inc 1

#include"post_file.hpp"

#include"SDL_types.h"
#include<string>
#include<vector>

using std::string;
using std::vector;

class PostSet {
    public:
        PostSet(int bar);
        ~PostSet(); 
    
        int Foo(void);

        string subject;
        vector<PostFile *> files;
        
    private:
    protected:
};

#endif
