#include "file_handle.hpp"

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include "console.hpp"

vector <FileHandle *> open_files;

FileHandle::FileHandle(string filename)
{
    struct stat my_stats;
    console->log("Opening " + filename);
    if(stat(filename.c_str(), &my_stats) == -1){
        fh = fopen(filename.c_str(), "w");
    } else {
        fh = fopen(filename.c_str(), "r+");
        size = my_stats.st_size;
    }
    pos = 0;
    this->filename = filename;
    ref_count = 0;
    still_open = 1;
}

FileHandle::~FileHandle()
{
    fclose(fh);
}

string FileHandle::get_line()
{
    char *sb = fgets(line_buffer, FH_BUFSIZE, fh);
    if(sb) {
        string s = sb;
        if('\n' == s[s.length() -1]) {
            s = s.substr(0, s.length()-1);
        }
        return s;
    } else {
        still_open = 0;
    }
    return "";
}

char *FileHandle::read_whole_file()
{
    char *buf = (char *)malloc(size);
    fread(buf, size, 1, fh);
    return buf;
}

void FileHandle::write_x_bytes_at(Uint32 x, Uint32 at, const char *data)
{
    if(pos != at){
        lseek(fileno(fh), at, SEEK_SET);
        pos = at;
    }

    fwrite(data, 1, x, fh);
    pos += x;
}

void FileHandle::close()
{
    ref_count--;
}

FileHandle *open_filehandle(string filename)
{
    Uint32 max_no = open_files.size();
    for(Uint32 i=0; i<max_no; ++i){
        if(0 == filename.compare(open_files[i]->filename)){
            open_files[i]->ref_count++;
            return open_files[i];
        }
    }

    FileHandle *handle = new FileHandle(filename);
    handle->ref_count++;
    open_files.push_back(handle);
    return handle;
}

void close_finished_files(void)
{
    vector < FileHandle *> still_open_files;
    Uint32 max_no = open_files.size();
    for(Uint32 i=0; i<max_no; i++){
        if(open_files[i]->ref_count){
            still_open_files.push_back(open_files[i]);
        }else{
            delete open_files[i];
        }
    }

    open_files = still_open_files;
}

void FileHandle::overwrite()
{
    fclose(fh);
    fh = fopen(filename.c_str(), "w");
    size = 0;
}
