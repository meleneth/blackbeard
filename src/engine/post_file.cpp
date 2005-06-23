#include "post_file.hpp"
#include"globals.hpp"

PostFile::PostFile() 
{
    num_pieces = 0;
}
    
PostFile::~PostFile() 
{
}

void PostFile::part(Uint32 part_no, Uint32 max_part_no, string message_id)
{
    if(num_pieces == 0){
        num_pieces = max_part_no;
    }else{
        if(max_part_no != num_pieces){
            console->log("Unhandled error: num pieces changed on message");
        }
        if(part_no > max_part_no){
            console->log("Not adding nonsensicle part no");
        }else{
            pieces[part_no] = message_id;
            seen_pieces++;
        }
    }
}

string PostFile::status(void)
{
    return "XX/XXX pieces seen";
}
