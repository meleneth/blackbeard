#ifndef ___verifypostfilejob_inc
#define ___verifypostfilejob_inc 1

#include "SDL_types.h"

#include "md5_stuff.hpp"
#include "job.hpp"
#include "file_handle.hpp"
#include "post_file.hpp"

#define VPFJ_BUFFER_SIZE 256000

class VerifyPostFileJob : public Job {
    public:
        VerifyPostFileJob(PostFile *file);
        ~VerifyPostFileJob();

        virtual void process();

        PostFile *post_file;
        FileHandle *file;
        MD5_CTX context;
        unsigned char hash[16];
        char big_buffer[VPFJ_BUFFER_SIZE];
    private:
    protected:
};

#endif
