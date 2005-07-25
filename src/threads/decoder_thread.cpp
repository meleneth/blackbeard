#include "decoder_thread.hpp"
#include"jobqueue.hpp"
#include"console.hpp"

using std::stringstream;

// Public data members go here.
DecoderThread::DecoderThread() // Constructor
{
}
    
DecoderThread::~DecoderThread() // Destructor
{
}

void DecoderThread::Execute(void)
{
    Decoder *decoder;
    while(1){
        decoder = (Decoder *)jobqueue->get_next_decoder_job();
        if(NULL != decoder){
            console->log("I gotta job!");
            NewsGroupPost *post = decoder->post;
            decoder->decode();
            stringstream buf;
            buf << "Decoder decoded " << decoder->num_bytes_written << " bytes";
            console->log(buf.str());
            delete decoder;
            delete post;
        }
    }
}

// Private members go here.
// Protected members go here.
