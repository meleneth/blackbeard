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

    easy_match = new StringPattern();
    easy_match->add_breaker(" - File ");
    easy_match->add_breaker(" of ");
    easy_match->add_breaker(": \"");
    easy_match->add_breaker("\" yEnc (");
    easy_match->add_breaker("/");
    easy_match->add_breaker(")");
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
    if(easy_match->does_match(subject)){
        vector<string> pieces;
        easy_match->pieces(subject, pieces);
        newsgroup->postset_for_subject(pieces[0])
                 ->file(pieces[1], pieces[2], pieces[3])
                 ->part(pieces[4], pieces[5]);
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
