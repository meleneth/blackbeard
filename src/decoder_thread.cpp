#include "decoder_thread.hpp"
#include"globals.hpp"

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
        decoder = jobqueue->get_next_decoder_job();
        if(NULL != decoder){
            console->log("I gotta job!");
            decoder->decode();
            stringstream buf;
            buf << "Decoder decoded " << decoder->num_bytes_written << " bytes";
            console->log(buf.str());
            delete decoder;
        }
    }
}

// Private members go here.
// Protected members go here.
