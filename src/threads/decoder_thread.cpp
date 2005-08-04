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
    Job *decoder;
    while(1){
        decoder = jobqueue->get_next_decoder_job();
        if(NULL != decoder){
            console->log("I gotta job!");
            decoder->process();
            jobqueue->finish(decoder);
            delete decoder;
        }
    }
}

// Private members go here.
// Protected members go here.
