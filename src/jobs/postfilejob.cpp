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
#include "verifypostfilejob.hpp"
#include "strutil.hpp"
#include "config.hpp"
#include "console.hpp"
#include "netcentral.hpp"
#include "database.hpp"


PostfileJob::PostfileJob(PostFile* post_file)
{
    postfile = post_file;
    job = NULL;
    postfile->status = "Queued";
    job_type = POSTFILE_DOWNLOAD;
    piece = postfile->pieces.begin();
    if(postfile->pieces.size() == 0){
        finish();
    }
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
        if(piece!=postfile->pieces.end()){
            PIECE_STATUS s = (*piece)->status;
            postfile->status = "Downloading";
            postfile->tick = config->tick;
            postfile->post_set->tick = config->tick;
            switch(s){
                case DOWNLOADING:
                    (*piece)->change_status(FINISHED);
                    postfile->_num_downloaded_pieces++;
                case MISSING:
                case DECODING:
                case FINISHED:
                    break;
                case SEEN:
                    (*piece)->change_status(DOWNLOADING);
                    BodyRetrieverJob *new_job = new BodyRetrieverJob(postfile, *piece);
                    new_job->srv = srv;
                    return new_job;
            }
            ++piece;
            if(piece == postfile->pieces.end()){
                postfile->status = "Finished";
                if(!is_par(postfile->filename)){
                    jobqueue->add_job(new VerifyPostFileJob(postfile));
                }
                //console->log("Finished getting file " + postfile->filename); 
                finish();
            }
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
