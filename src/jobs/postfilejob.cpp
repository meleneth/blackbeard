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

#include "postfilejob.hpp"
#include "nntpserver.hpp"
#include "bodyretrieverjob.hpp"
#include "headersforgroupjob.hpp"
#include "strutil.hpp"
#include "config.hpp"
#include "console.hpp"
#include "netcentral.hpp"
#include "database.hpp"

PostfileJob::PostfileJob(PostFile* post_file)
{
    postfile = post_file;
    job = NULL;
    piece_no = 0;
    postfile->status = "Queued";
    job_type = POSTFILE_DOWNLOAD;
}

PostfileJob::~PostfileJob()
{
}

Job* PostfileJob::get_next_job()
{

    if(is_finished){
        return NULL;
    }

    if(high_priority_jobs->has_more_jobs()){
        console->log("Triggered high_priority_job in PostfileJob::get_next_job()");
        return high_priority_jobs->get_next_job();
    }

    if(postfile){
        postfile->needs_full_info(); 
        PIECE_STATUS s = postfile->pieces[piece_no]->status;
        postfile->status = "Downloading";
        postfile->tick = config->tick;
        switch(s){
            case DOWNLOADING:
                postfile->pieces[piece_no]->change_status(FINISHED);
                postfile->_num_downloaded_pieces++;
            case MISSING:
            case DECODING:
            case FINISHED:
                break;
            case SEEN:
                postfile->pieces[piece_no]->change_status(DOWNLOADING);
                BodyRetrieverJob *new_job = new BodyRetrieverJob(postfile, postfile->pieces[piece_no]);
                new_job->srv = srv;
                return new_job;
        }
        if(++piece_no == postfile->pieces.size()){
            postfile->status = "Finished";
            finish();
        }
    } else {
        finish();
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
