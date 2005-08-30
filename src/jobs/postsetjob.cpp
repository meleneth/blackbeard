#include "postsetjob.hpp"
#include"nntpserver.hpp"
#include"bodyretrieverjob.hpp"

using std::string;
using std::stringstream;
#include<sstream>

PostsetJob::PostsetJob(PostSet* post_set)
{
    postset = post_set;
    job = NULL;
    file_no = 0;
    piece_no = 0;
}

PostsetJob::~PostsetJob()
{
}

Job* PostsetJob::get_next_job()
{
    if(is_finished){
        return NULL;
    }

    PostFile *file = postset->files[file_no];
    PIECE_STATUS s = file->piece_status[piece_no];
    switch(s){
        case MISSING:
        case DOWNLOADING:
        case DECODING:
        case FINISHED:
            if(++piece_no > file->num_pieces){
                piece_no = 0;
                if(++file_no > postset->num_files){
                    is_finished = 1;
                    return NULL;
                }
            }
            break;
        case SEEN:
            file->piece_status[piece_no] = DOWNLOADING;
            return new BodyRetrieverJob(file, file->pieces[piece_no]);
    }
    return NULL;
}

Uint32 PostsetJob::pieces_left_to_download()
{
    return (postset->num_finished_pieces < postset->num_pieces);
}

void PostsetJob::process()
{
    if(job){
        job->process();
        if(job->is_finished){
            delete job;
            job = NULL;
        }
    }

    if(!job){
        job = get_next_job();
    }
}

string PostsetJob::status_line(void)
{
    stringstream buf;
    buf << "Download job for " << postset->subject;
    return buf.str();
}
