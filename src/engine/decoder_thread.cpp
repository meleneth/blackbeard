#include "decoder_thread.hpp"
#include"globals.hpp"

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
        }
    }
}

// Private members go here.
// Protected members go here.
