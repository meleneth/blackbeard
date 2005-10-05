#include<sstream>
#include<iostream>  // I/O 
#include<fstream>   // file I/O
#include<sstream>

using std::string;
using std::stringstream;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::endl;

#include"postfilejob.hpp"
#include"nntpserver.hpp"
#include"bodyretrieverjob.hpp"
#include"headersforgroupjob.hpp"
#include"strutil.hpp"
#include"config.hpp"
#include"console.hpp"

PostfileJob::PostfileJob(PostFile* post_file)
{
    postfile = post_file;
    job = NULL;
    piece_no = 0;
    postfile->status = "Queued";
}

PostfileJob::~PostfileJob()
{
}

Job* PostfileJob::get_next_job()
{
    if(is_finished){
        return NULL;
    }

    if(postfile){
        PIECE_STATUS s = postfile->piece_status[piece_no];
        switch(s){
            case MISSING:
            case DOWNLOADING:
            case DECODING:
            case FINISHED:
                break;
            case SEEN:
                postfile->piece_status[piece_no] = DOWNLOADING;
                BodyRetrieverJob *new_job = new BodyRetrieverJob(postfile, postfile->pieces[piece_no]);
                new_job->srv = srv;
                return new_job;
        }
        if(++piece_no > postfile->num_pieces){
            is_finished = 1;
        }
    } else {
        is_finished = 1;
    }

    return NULL;
}

void PostfileJob::process()
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

string PostfileJob::status_line(void)
{
    stringstream buf;
    buf << "Download job for " << postfile->filename;
    return buf.str();
}
