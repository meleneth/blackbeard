#include "post_set.hpp"
#include"globals.hpp"

#include<sstream>
#include<iomanip>
using std::stringstream;
using std::setprecision;

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

double PostSet::completed_percent(void)
{
    Uint32 total_pieces = 0;
    Uint32 downloaded_pieces = 0;
    vector<PostFile *>::iterator v;

    for (v = files.begin(); v != files.end(); ++v){
        if(*v){
            total_pieces += (*v)->num_pieces;
            downloaded_pieces += (*v)->downloaded_pieces;
        }
    }

    if(total_pieces == downloaded_pieces)
        return 100;

    return ((double)downloaded_pieces / (double)total_pieces * (double) 100);
}

PostFile *PostSet::file(Uint32 file_num, Uint32 max_file_num, string file_name)
{
    if(!num_files){
        num_files = max_file_num;
        files.resize(num_files + 1);
        Uint32 i;
        for(i=0;i<num_files+1;i++){
            files[i] = NULL;
        }
    }

    if(files[file_num]){
        return files[file_num];
    }
    
    files[file_num] = new PostFile(this);
    files[file_num]->filename = file_name;
    return files[file_num];
}

string PostSet::status(void)
{
    stringstream buf;
    buf << subject << ", " << num_files << " files. " << setprecision(3) << completed_percent() << "%";
    return buf.str();
}
