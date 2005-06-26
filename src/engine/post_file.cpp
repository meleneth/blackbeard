#include "post_file.hpp"
#include"globals.hpp"

#include<sstream>

using std::stringstream;

PostFile::PostFile() 
{
    num_pieces = 0;
    seen_pieces = 0;
    downloaded_pieces = 0;
}
    
PostFile::~PostFile() 
{
}

void PostFile::part(Uint32 part_no, Uint32 max_part_no, string message_id)
{
    if(num_pieces == 0){
        num_pieces = max_part_no;
        pieces.resize(num_pieces + 1);

        pieces[max_part_no] = "";
        while(max_part_no){
            max_part_no--;
            pieces[max_part_no] = "";
        }
        max_part_no = num_pieces;
    }

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

string PostFile::status(void)
{
    stringstream status;
    status << filename << " - ";
    status << seen_pieces << "/" << num_pieces << " pieces seen   " 
           << downloaded_pieces << "/"  << num_pieces << " pieces downloaded  ";
    if(num_pieces > 0)
           status << ((double)downloaded_pieces / (double)num_pieces) * (double) 100
                  << "%";
    return status.str();
}
