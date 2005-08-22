#include "bodyretrieverjob.hpp"
#include"nntpserver.hpp"
#include"console.hpp"
#include"yenc_decoder.hpp"
#include"uu_decoder.hpp"
#include"jobqueue.hpp"

BodyRetrieverJob::BodyRetrieverJob(NewsGroup *group, PostFile *file, Uint32 msg_id)
{
    net_cmds.push_back("group " + group->name);
    net_cmds.push_back("body " + msg_id);

    post = new NewsGroupPost();
    this->file = file;
    this->msg_id = msg_id;
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
    return "Default status for BodyRetrieverJob";
}

void BodyRetrieverJob::finish()
{
    console->log("Finished with body retrieval - should spawn decodder here");
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
    NetJob::finish();
}
