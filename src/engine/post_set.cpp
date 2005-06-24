#include "post_set.hpp"
#include"globals.hpp"

#include<sstream>
using std::stringstream;

PostSet::PostSet(string subject) 
{
    this->subject = subject;
    num_files = 0;
    num_pieces = 0;
    num_finished_pieces = 0;
}
    
PostSet::~PostSet() 
{
}

string PostSet::completed_percent(void)
{
    return " 00.00%";
}

PostFile *PostSet::file(Uint32 file_num, Uint32 max_file_num, string file_name)
{
    num_files = max_file_num;
    return new PostFile();
}

string PostSet::status(void)
{
    stringstream buf;
    buf << subject << ", " << num_files << " files. " << completed_percent();
    return buf.str();
}
