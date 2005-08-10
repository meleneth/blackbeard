#include "subjectfileloaderjob.hpp"
#include"console.hpp"
#include"config.hpp"
#include"newsgroup.hpp"

#include <iostream>  // I/O 
#include <fstream>   // file I/O
#include<sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include<iomanip>

#define MAX_SUBJECTS_PER_TICK 2000

using std::string;
using std::stringstream;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::endl;
using std::setprecision;

SubjectFileLoaderJob::SubjectFileLoaderJob(string filename)
{
    this->filename = filename;

    total_bytes = 0;
    console->log("Loading subjects from " + filename);
    struct stat my_stats;
    if(stat(filename.c_str(), &my_stats) == -1){
        return;
    }
    total_bytes = my_stats.st_size;
    newsgroup = group_for_name("alt.mama");
    newsgroups.push_back(newsgroup);

    in.open(filename.c_str(), ios::in);
    in.getline(linebuffer, 1024);
}

SubjectFileLoaderJob::~SubjectFileLoaderJob()
{
}

string SubjectFileLoaderJob::status_line(void)
{
    stringstream buf;
    buf << "Reading subjects from " << filename << " (" << bytes_read << "/" << total_bytes << ")";
    if(total_bytes)
        buf << " - " << setprecision(2) << ((double)bytes_read / (double) total_bytes) * (double)100 << "%";
    return buf.str();
}

void SubjectFileLoaderJob::process(void)
{
    if(is_finished)
        return;

    Uint32 ctr = MAX_SUBJECTS_PER_TICK;
    while(!in.eof()){
        bytes_read += strlen(linebuffer);
        newsgroup->digest_subject_line("stored", linebuffer);
        in.getline(linebuffer, 1024);
        if(!ctr--)
            return;
    }
    is_finished = 1; 
    console->log("done loading subjects");
}
