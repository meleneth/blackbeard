#ifndef ___pararchive_inc
#define ___pararchive_inc 1

#include "SDL_types.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class ParArchive {
    public:
        ParArchive(string filename);
        ~ParArchive();

        void add_parfile(string filename);
        Uint32 exists_in_par(string filename);
        Uint32 file_is_corrupt(string filename);

        vector<string> par_filenames;

        int is_corrupt;

    private:
    protected:
};


ParArchive *load_par_file(string filename);
int is_par(string filename);
int is_base_par(string filename);
int is_recovery_par(string filename);

#endif
