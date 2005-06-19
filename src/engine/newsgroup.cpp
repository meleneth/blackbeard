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

void NewsGroup::digest_subject_line(std::string message_id, std::string subject)
{
    console->log("(" + message_id + ") " + subject);
}

int NewsGroup::status_code()
{
	return return_code;
}

// Private members go here.
// Protected members go here.
