#include "newsgroup.hpp"
#include "news_constants.hpp"
#include "globals.hpp"
#include "strutil.hpp"
#include <iostream>  // I/O 
#include <fstream>   // file I/O

using std::string;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::endl;

// Public data members go here.
NewsGroup::NewsGroup(string group_name) // Constructor
{
    name = group_name;
    console->log("Creation of object for " + group_name);
  
    StringPattern *pattern;
//1997-11-19 Atripolis_2097-MIRAGE "ATRIPOLI.R10" (4/4) yEnc
    pattern = new StringPattern(SP_LASTPART +1);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(") yEnc");
    yenc_subject_patterns.push_front(pattern);
    pattern->name = "Harold";
//001 - Initial D S4D1 (2-9) - yEnc "HD1_5.part001.rar" (01/79)
    pattern = new StringPattern(SP_LASTPART +1);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" - yEnc \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    yenc_subject_patterns.push_front(pattern);
    pattern->name = "zPimplez";

// Beekmans post "fUf.v4.pal.dvdr.part024.rar" [027/144] - yEnc (140/201)
    pattern = new StringPattern(SP_LASTPART +1);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" [");
    pattern->add_breaker(SP_FILENO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXFILENO);
    pattern->add_breaker("] - yEnc (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    yenc_subject_patterns.push_front(pattern);
    pattern->name = "Maude";


// another fine blb post - File 1 of 1: "blb" yEnc (1/4)
    pattern = new StringPattern(SP_LASTPART +1);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" - File ");
    pattern->add_breaker(SP_FILENO);
    pattern->add_breaker(" of ");
    pattern->add_breaker(SP_MAXFILENO);
    pattern->add_breaker(": \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" yEnc (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    yenc_subject_patterns.push_front(pattern);
    pattern->name = "Harriet";
    
//new and improved blackbeard arrr - "blb" yEnc (1/6)
    pattern = new StringPattern(SP_LASTPART +1);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker("- \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" yEnc (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    yenc_subject_patterns.push_front(pattern);
    pattern->name = "Matilda";

//(OMNI) E's Otherwise vol.3 [107/114] - "jspec-es.otherwise.vol3.vol0189+131.PAR2" yEnc (054/137)
//(OMNI) Case.Closed.Case.04.vol.05.r1.dvdr-kif[30/97] - "case.closed.case.04.vol.05.r1.dvdr-kif.r27" yEnc (072/201)
    pattern = new StringPattern(SP_LASTPART +1);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker("[");
    pattern->add_breaker(SP_FILENO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXFILENO);
    pattern->add_breaker("] - \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" yEnc (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    yenc_subject_patterns.push_front(pattern);
    pattern->name = "switch";

//UUDecode patterns

//SDL for those in need - SDL-1.2.7.tar.gz (1/8)
    pattern = new StringPattern(SP_LASTPART +1);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" - ");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker(" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    uu_subject_patterns.push_front(pattern);
    pattern->name = "Stumpy";

//SDL for those in need - File 1 of 1: SDL-1.2.7.tar.gz (1/8)
    pattern = new StringPattern(SP_LASTPART +1);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" - File ");
    pattern->add_breaker(SP_FILENO);
    pattern->add_breaker(" of ");
    pattern->add_breaker(SP_MAXFILENO);
    pattern->add_breaker(": ");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker(" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    uu_subject_patterns.push_front(pattern);
    pattern->name = "Pally";

//Fooly Cooly Volume 1 ~ "FLCL 1.part18.rar" ~[21/90] - REQ Kaleido Star Volumes 1,2 and 3 (085/127)
    pattern = new StringPattern(SP_LASTPART +2);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" ~ \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" ~[");
    pattern->add_breaker(SP_FILENO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXFILENO);
    pattern->add_breaker("] - ");
    pattern->add_breaker(" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    uu_subject_patterns.push_front(pattern);
    pattern->name = "Schnell";
}

    
NewsGroup::~NewsGroup() // Destructor
{
}

void NewsGroup::header_scoop(string xover_line)
{
    vector<string> header_pieces;
    vector<string>::iterator i;

    Tokenize(xover_line, header_pieces, "\t");

    digest_subject_line(header_pieces[0], header_pieces[1]);

}

void NewsGroup::digest_subject_line(string message_id, string subject)
{
    list< StringPattern * >::iterator sp;
    PostSet *current_postset;
    console->log("Subject: " + subject);

    //"[DVD9]" evil delete it >.<
    size_t s = subject.find("[DVD9]");
    while(s < string::npos){
        subject.replace(s, 6, "_DVD9_");
        s=subject.find("[DVD9]");
    }

    for (sp = yenc_subject_patterns.begin(); sp != yenc_subject_patterns.end(); ++sp){
        if((*sp)->match(subject)){
            current_postset = newsgroup->postset_for_subject((*sp)->get_piece(SP_SUBJECT));
            if(console->current_postset == NULL)
                console->current_postset = current_postset;

            current_postfile = current_postset->file((*sp)->get_piecen(SP_FILENO), 
                                                     (*sp)->get_piecen(SP_MAXFILENO), 
                                                     (*sp)->get_piece(SP_FILENAME));

            current_postfile->decoder_type = DT_YENC;
            current_postfile->part((*sp)->get_piecen(SP_PARTNO), 
                                   (*sp)->get_piecen(SP_MAXPARTNO), message_id);
            return;
        }
    }

    for (sp = uu_subject_patterns.begin(); sp != uu_subject_patterns.end(); ++sp){
        if((*sp)->match(subject)){
            current_postset = newsgroup->postset_for_subject((*sp)->get_piece(SP_SUBJECT));
            if(console->current_postset == NULL)
                console->current_postset = current_postset;
            current_postfile = current_postset->file((*sp)->get_piecen(SP_FILENO), 
                                                     (*sp)->get_piecen(SP_MAXFILENO), 
                                                     (*sp)->get_piece(SP_FILENAME));
            current_postfile->decoder_type = DT_UUDECODE;
            current_postfile->part((*sp)->get_piecen(SP_PARTNO), 
                                   (*sp)->get_piecen(SP_MAXPARTNO), message_id);
            return;
        }
    }
}

PostSet *NewsGroup::postset_for_subject(string subject)
{
    list<PostSet *>::iterator i;
    for(i = postsets.begin() ; i != postsets.end() ; ++i){
        if(0 == subject.compare((*i)->subject)){
            return *i;
        }
    }

    PostSet *new_post = new PostSet(subject);
    postsets.push_back(new_post);
    return new_post;
}

int NewsGroup::status_code()
{
	return return_code;
}

void NewsGroup::load_from_file(string filename)
{
    char linebuffer[1024];
    ifstream in;

    in.open(filename.c_str(), ios::in);
    in.getline(linebuffer, 1024);

    while(!in.eof()){
        digest_subject_line("stored", linebuffer);
        in.getline(linebuffer, 1024);
    }
}

