#ifndef ___getxover_inc
#define ___getxover_inc 1

#include"SDL_types.h"

#include<string>

using std::string;


#define XOVER_CHUNK_SIZE 5000

class GetXover {
    public:
        GetXover();
        ~GetXover();

        string group_name;
        Uint32 lower_id;
        Uint32 upper_id;
    private:
    protected:
};

#endif
