#include "bodyretrieverjob.hpp"
#include "nntpserver.hpp"
#include "console.hpp"
#include "jobqueue.hpp"
#include "decoderfactory.hpp"

#include <sstream>

using std::stringstream;

BodyRetrieverJob::BodyRetrieverJob(PostFile *file, FilePiece *piece)
{
    stringstream cmd;
    cmd << "body " << piece->msg_id;
    group = file->post_set->group;
    net_cmds.push_back(cmd.str());

    post = new NewsGroupPost();
    this->file = file;
    this->piece = piece;
    job_type = BODY_DOWNLOAD;
}

BodyRetrieverJob::~BodyRetrieverJob()
{
}

void BodyRetrieverJob::process_line(string line)
{
    post->lines.push_back(line);
}

string BodyRetrieverJob::status_line(void)
{
    return "Default status for BodyRetrieverJob: " + file->get_bar();
}

void BodyRetrieverJob::finish()
{
    if(post->lines.size()){
        Decoder *decoder = decoder_for_body(post, file);
        if(decoder){
            jobqueue->jobs.push_back(decoder);
        }
    }else{
        delete post;
    }
    NetJob::finish();
}
