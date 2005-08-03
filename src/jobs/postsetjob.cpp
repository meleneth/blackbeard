#include "postsetjob.hpp"
#include"nntpserver.hpp"

using std::string;
using std::stringstream;
#include<sstream>

PostsetJob::PostsetJob(PostSet* post_set)
{
    postset = post_set;
}

PostsetJob::~PostsetJob()
{
}

Job* PostsetJob::get_next_job()
{
	if (pieces_left_to_download() > 0) {
		Job* job = new Job();
		return job; 
	}
    return NULL;
}

Uint32 PostsetJob::pieces_left_to_download()
{
    return (postset->num_finished_pieces < postset->num_pieces);
}

void PostsetJob::process(void *connection)
{
    NNTPServer *srv = (NNTPServer *)connection;
}

string PostsetJob::status_line(void)
{
    stringstream buf;
    buf << "Download job for " << postset->subject;
    return buf.str();
}
