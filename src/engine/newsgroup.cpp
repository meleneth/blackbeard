#include "newsgroup.hpp"
#include"news_constants.hpp"
#include"globals.hpp"
#include"strutil.hpp"

using std::string;

// Public data members go here.
NewsGroup::NewsGroup(string group_name) // Constructor
{
    name = group_name;
}
    
NewsGroup::~NewsGroup() // Destructor
{
}

void NewsGroup::header_scoop(string xover_line)
{
    vector<string> header_pieces;
    vector<string>::iterator i;

    Tokenize(xover_line, header_pieces, "\t");

    for(i = header_pieces.begin() ; i != header_pieces.end() ; ++i){
        console->log(*i);
    }
}

int NewsGroup::status_code()
{
	return return_code;
}

// Private members go here.
// Protected members go here.
