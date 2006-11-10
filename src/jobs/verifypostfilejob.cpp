#include <sstream>

using std::stringstream;

#include "console.hpp"
#include "verifypostfilejob.hpp"

VerifyPostFileJob::VerifyPostFileJob(PostFile *file)
{
    post_file = file;
    this->file = post_file->open_file();
    fseek(this->file->fh, 0, SEEK_SET); // stupid, FileHandle not doing it's job
    MD5_Init(&context);
}

VerifyPostFileJob::~VerifyPostFileJob()
{
}

void VerifyPostFileJob::process(void)
{
    if(is_finished)
        return;
    
    size_t bytes_read = fread(big_buffer, 1, VPFJ_BUFFER_SIZE, file->fh);
    MD5_Update(&context, big_buffer, bytes_read);
    if(feof(file->fh)) {
        is_finished = 1;
        MD5_Final(hash, &context);
        if(!CMP_MD5(hash, post_file->hash)) {
            stringstream s;
            s << "File hash of " << hash_to_hex(hash) << " doesn't match stored value of " << hash_to_hex(post_file->hash);
            console->log(s.str());
            post_file->status = "CORRUPT";
        }
        file->close();
        close_finished_files();
    }

}

string VerifyPostFileJob::status_line(void)
{
    stringstream s;
    s << "VerifyPostFileJob: " << is_finished;
    return s.str();
}

