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

#include"postsetjob.hpp"
#include"nntpserver.hpp"
#include"bodyretrieverjob.hpp"
#include"headersforgroupjob.hpp"
#include"strutil.hpp"
#include"config.hpp"
#include"console.hpp"

PostsetJob::PostsetJob(PostSet* post_set)
{
    postset = post_set;
    job = NULL;
    file_no = 0;
    piece_no = 0;
    job_status_filename = get_crc_32(post_set->subject);
}

PostsetJob::PostsetJob(string filename)
{
    job = NULL;
    file_no = 0;
    piece_no = 0;
    job_status_filename = filename;
    load_job_status();
}

PostsetJob::~PostsetJob()
{
}

Job* PostsetJob::get_next_job()
{
    if(is_finished){
        return NULL;
    }

    if(!postset->has_msg_ids){
        return new HeadersForGroupJob(postset->group, postset->_min_msg_id, postset->_max_msg_id);
    }

    PostFile *file = postset->files[file_no];
    if(file){
        PIECE_STATUS s = file->piece_status[piece_no];
        switch(s){
            case MISSING:
            case DOWNLOADING:
            case DECODING:
            case FINISHED:
                break;
            case SEEN:
                file->piece_status[piece_no] = DOWNLOADING;
                BodyRetrieverJob *new_job = new BodyRetrieverJob(file, file->pieces[piece_no]);
                new_job->srv = srv;
                return new_job;
        }
        if(++piece_no > file->num_pieces){
            piece_no = 0;
            ++file_no;
        }
    } else {
        piece_no = 0;
        ++file_no;
    }

    if(file_no > postset->num_files){
        is_finished = 1;
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
            save_job_status();
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

void PostsetJob::save_job_status(void)
{
    ofstream out;

    out.open(config->full_job_filename(job_status_filename).c_str(), ios::out);

    if(out.is_open()){
        out << postset->subject << endl;
        Uint32 max_no = postset->files.size();
        for(Uint32 i=0; i<max_no; i++)
        {
            PostFile *file = postset->files[i]; 
            if(file){
                out << "FILE: " << file->num_pieces << " " << file->filename << endl;
                Uint32 max_piece_no = file->pieces.size();
                for(Uint32 j=0 ; j<max_piece_no ; j++){
                    out << file->piece_status[j] << " " << file->pieces[j] << endl;
                }
            }
        }
    }
}

void PostsetJob::load_job_status(void)
{
    StringPattern *filepattern = new StringPattern(4);
    filepattern->add_breaker(0);
    filepattern->add_breaker("FILE: ");
    filepattern->add_breaker(1);
    filepattern->add_breaker(" ");
    filepattern->add_breaker(2);
    
    StringPattern *piecepattern = new StringPattern(2);
    piecepattern->add_breaker(0);
    piecepattern->add_breaker(" ");
    piecepattern->add_breaker(1);
    
    ifstream in;
    in.open(config->full_job_filename(job_status_filename).c_str(), ios::in);
    
    char linebuffer[1024];
    PostFile * file = NULL;

    if(in.is_open()){
        in.getline(linebuffer, 1024);
        postset = new PostSet(linebuffer);
        
        while(!in.eof()){
            if(strlen(linebuffer)){
                if(filepattern->match(linebuffer)){
                    file = new PostFile(postset);
                    file->filename = filepattern->get_piece(2);
                    postset->files.push_back(file);
                } else {
                    if(piecepattern->match(linebuffer)){
                        file->pieces.push_back(piecepattern->get_piecen(1));
                        file->piece_status.push_back((PIECE_STATUS) piecepattern->get_piecen(0));
                    }
                }  
            }
            in.getline(linebuffer, 1024);
        }
    } else {
        console->log("File open failed!");
    }
}

