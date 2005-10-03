#ifndef ___file_handle_inc
#define ___file_handle_inc 1

#include "SDL_types.h"
#include <vector>
#include <string>

using std::string;
using std::vector;

class FileHandle {
    public:
        FileHandle(string filename);
        ~FileHandle();

        string get_line();
        void write_x_bytes_at(Uint32 x, Uint32 at, const char *data);
        void close();

        string filename;
        Uint32 ref_count;
        Uint32 pos;
        FILE *fh;
        char line_buffer[4096];
    private:
    protected:
};

FileHandle *open_file(string filename);
void close_finished_files(void);

#endif
