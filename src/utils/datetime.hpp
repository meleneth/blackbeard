#ifndef ___datetime_inc
#define ___datetime_inc 1

#include "SDL_types.h"
#include <string>

using std::string;

class DateTime {
    public:
        DateTime();
        ~DateTime();

        string as_string(string date_format);

    private:
    protected:
};

#endif
