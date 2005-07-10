#include "newsgroup.hpp"
#include"news_constants.hpp"
#include"globals.hpp"
#include"strutil.hpp"

using std::string;

// Public data members go here.
NewsGroup::NewsGroup(string group_name) // Constructor
{
    name = group_name;
    console->log("Creation of object for " + group_name);
  
    StringPattern *pattern;
//1997-11-19 Atripolis_2097-MIRAGE "ATRIPOLI.R10" (4/4) yEnc
    pattern = new StringPattern(SP_LASTPART);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(") yEnc");
    yenc_subject_patterns.push_front(pattern);

// another fine blb post - File 1 of 1: "blb" yEnc (1/4)
    pattern = new StringPattern(SP_LASTPART);
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
    
//new and improved blackbeard arrr - "blb" yEnc (1/6)
    pattern = new StringPattern(SP_LASTPART);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker("- \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" yEnc (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    yenc_subject_patterns.push_front(pattern);

//UUDecode patterns
//SDL for those in need - SDL-1.2.7.tar.gz (1/8)
    pattern = new StringPattern(SP_LASTPART);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" - ");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker(" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(")");
    uu_subject_patterns.push_front(pattern);

//SDL for those in need - File 1 of 1: SDL-1.2.7.tar.gz (1/8)
    pattern = new StringPattern(SP_LASTPART);
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

// Private members go here.
// Protected members go here.
