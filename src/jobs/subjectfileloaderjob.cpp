#include "subjectfileloaderjob.hpp"
#include"console.hpp"
#include"config.hpp"
#include"newsgroup.hpp"

#include <iostream>  // I/O 
#include <fstream>   // file I/O
#include<sstream>

using std::string;
using std::stringstream;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::endl;

SubjectFileLoaderJob::SubjectFileLoaderJob(string filename)
{
    this->filename = filename;
}

SubjectFileLoaderJob::~SubjectFileLoaderJob()
{
}

string SubjectFileLoaderJob::status_line(void)
{
    return "Reading subjects from " + filename;
}

void SubjectFileLoaderJob::process(void)
{
    console->log("Loading subjects from " + filename);
    NewsGroup *newsgroup = group_for_name("alt.mama");
    newsgroups.push_back(newsgroup);
    char linebuffer[1024];
    ifstream in;

    in.open(filename.c_str(), ios::in);
    in.getline(linebuffer, 1024);

    while(!in.eof()){
        newsgroup->digest_subject_line("stored", linebuffer);
        in.getline(linebuffer, 1024);
    }
    console->log("done loading subjects");
}

