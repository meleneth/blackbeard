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
  
// another fine blb post - File 1 of 1: "blb" yEnc (1/4)
    
    pattern = new StringPattern(SP_LASTPART);
    pattern->add_breaker(" - File ");
    pattern->add_breaker(" of ");
    pattern->add_breaker(": \"");
    pattern->add_breaker("\" yEnc (");
    pattern->add_breaker("/");
    pattern->add_breaker(")");
    subject_patterns.push_front(pattern);
    
//new and improved blackbeard arrr - "blb" yEnc (1/6)
    
    pattern = new StringPattern(SP_LASTPART);
    pattern->add_breaker("- \"");
    pattern->add_breaker("\" yEnc (");
    pattern->add_breaker("/");
    pattern->add_breaker(")");
    subject_patterns.push_front(pattern);
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
    console->log("(" + message_id + ") " + subject);
    list< StringPattern * >::iterator sp;

    for (sp = subject_patterns.begin(); sp != subject_patterns.end(); ++sp){
        if((*sp)->does_match(subject)){
            (*sp)->pieces(subject);
            current_postset = newsgroup->postset_for_subject((*sp)->get_piece(SP_SUBJECT));
            current_postfile = current_postset->file(atoi((*sp)->get_piece(SP_FILENO).c_str()), 
                                                     atoi((*sp)->get_piece(SP_MAXFILENO).c_str()), 
                                                     (*sp)->get_piece(SP_FILENAME));
            current_postfile->part(atoi((*sp)->get_piece(SP_PARTNO).c_str()), 
                                   atoi((*sp)->get_piece(SP_MAXPARTNO).c_str()), message_id);
            console->log("matched subject pattern");
            printf("XXXXXXXXXX\n");
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
    postsets.push_front(new_post);
    return new_post;
}

int NewsGroup::status_code()
{
	return return_code;
}

// Private members go here.
// Protected members go here.
