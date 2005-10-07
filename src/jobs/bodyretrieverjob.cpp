#include "bodyretrieverjob.hpp"
#include"nntpserver.hpp"
#include"console.hpp"
#include"yenc_decoder.hpp"
#include"uu_decoder.hpp"
#include"jobqueue.hpp"

#include<sstream>

using std::stringstream;

BodyRetrieverJob::BodyRetrieverJob(PostFile *file, Uint32 msg_id)
{
    stringstream cmd;
    cmd << "body " << msg_id;
    group = file->post_set->group;
    net_cmds.push_back(cmd.str());

    post = new NewsGroupPost();
    this->file = file;
    this->msg_id = msg_id;
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
    file->downloaded_pieces++;
    if(post->lines.size()){
        switch(file->decoder_type){
            case DT_YENC:
                jobqueue->jobs.push_back(new yEncDecoder(post, file));
                break;
            case DT_UUDECODE:
                jobqueue->jobs.push_back(new UUDecoder(post, file, msg_id));
                break;
            case DT_MIME:
            case DT_UNKNOWN:
                console->log("Dunno what to do with this type of encoded material");
            break;
        }
    }else{
        delete post;
    }
    NetJob::finish();
}
