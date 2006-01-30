#ifndef ___newsgrouppost_inc
#define ___newsgrouppost_inc 1

#include "SDL_types.h"
#include "post_file.hpp"

#include <string>
#include <vector>

using std::vector;
using std::string;

class PostFile;
class NewsGroupPost {
    public:
        // Public data members go here.
        NewsGroupPost(); // Constructor
        ~NewsGroupPost(); // Destructor
        static bool compare(const NewsGroupPost *a, const NewsGroupPost *b);

        vector<string> lines;
        PostFile *post_file;
    private:
        // Private members go here.
    protected:
        // Protected members go here.
};

#endif
