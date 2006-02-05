#include <sstream>
#include <iostream>  // I/O 
#include <fstream>   // file I/O
#include <sstream>

using std::string;
using std::stringstream;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::endl;

#include "postsetjob.hpp"
#include "nntpserver.hpp"
#include "bodyretrieverjob.hpp"
#include "headersforgroupjob.hpp"
#include "strutil.hpp"
#include "config.hpp"
#include "console.hpp"
#include "session.hpp"
#include "netcentral.hpp"
#include "postfilejob.hpp"

PostsetJob::PostsetJob(PostSet* post_set)
{
    postset = post_set;
    job = NULL;
    file_no = 0;
    piece_no = 0;
    job_status_filename = get_crc_32(post_set->subject);
    get_headers = NULL;
    job_type = POSTSET_DOWNLOAD;

    make_downloader_children();
}

void PostsetJob::make_downloader_children(void)
{
    Uint32 max_file_no = postset->files.size();

    for(Uint32 i=0; i < max_file_no ; i++)
    {
        PostFile *f = postset->files[i];
        if(f){
            if(f->is_par()){
                f->status = "Skipped";
            }else{
                netcentral->add_job(new PostfileJob(f));
            }
        }
    }

}


PostsetJob::PostsetJob(string filename)
{
    job = NULL;
    file_no = 0;
    piece_no = 0;
    job_status_filename = filename;
    load_job_status();
    job_type = POSTSET_DOWNLOAD;
}

PostsetJob::~PostsetJob()
{
}

void PostsetJob::notify_child_finished(Job *job)
{
    if(job == get_headers) {
        make_downloader_children();
    } else {
        is_finished = 1;
    }
    Job::notify_child_finished(job);
}

void PostsetJob::process()
{
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
        out << postset->group->name << endl;
        Uint32 max_no = postset->files.size();
        for(Uint32 i=0; i<max_no; i++)
        {
            PostFile *file = postset->files[i]; 
            if(file){
                out << "FILE: " << file->pieces.size() << " " << file->filename << endl;
                Uint32 max_piece_no = file->pieces.size();
                for(Uint32 j=0 ; j<max_piece_no ; j++){
                    out << file->pieces[j]->status << " " << file->pieces[j]->msg_id << endl;
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
    filepattern->name = "File";
    
    StringPattern *piecepattern = new StringPattern(2);
    piecepattern->add_breaker(0);
    piecepattern->add_breaker(" ");
    piecepattern->add_breaker(1);
    piecepattern->name = "Piece";
    
    ifstream in;
    string f = config->full_job_filename(job_status_filename);
    console->log("Restoring job from " + f);

    in.open(f.c_str(), ios::in);
    
    char linebuffer[1024];
    PostFile * file = NULL;

    if(in.is_open()){
        in.getline(linebuffer, 1024);
        postset = new PostSet(linebuffer);
        postset->has_msg_ids = 1;
        in.getline(linebuffer, 1024);
        postset->group = group_for_name(linebuffer);
        postset->group->postsets.push_back(postset);
        in.getline(linebuffer, 1024);

        while(!in.eof()){
            if(strlen(linebuffer)){
                if(filepattern->match(linebuffer)){
                    file = new PostFile(postset);
                    file->filename = filepattern->get_piece(2);
                    file->decoder_type = DT_YENC;
                    postset->files.push_back(file);
                } else {
                    if(piecepattern->match(linebuffer)){
                        FilePiece *piece = new FilePiece( piecepattern->get_piecen(1), (PIECE_STATUS) piecepattern->get_piecen(0), file);
                        file->pieces.push_back(piece);
                    }
                }  
            }
            in.getline(linebuffer, 1024);
        }
        Uint32 max_no = postset->files.size();
        for(Uint32 i=0; i<max_no; ++i){
            postset->files[i]->update_status_from_pieces();
        }
    } else {
        console->log("File open failed!");
    }

    make_downloader_children();
}

