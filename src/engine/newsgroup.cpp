#include "newsgroup.hpp"
#include"news_constants.hpp"
#include"globals.hpp"


// Public data members go here.
NewsGroup::NewsGroup(std::string group_name) // Constructor
{
    name = group_name;
}
    
NewsGroup::~NewsGroup() // Destructor
{
}

void NewsGroup::header_scoop(std::string xover_line)
{
    std::list <std::string> header_pieces;

    size_t start, end;
    start = 0;
    end = 0;

    while(end < xover_line.length()){
        end++;
        if(xover_line[end] == ' '){
            std::string line_bit = xover_line.substr(start, end);
            header_pieces.push_back(line_bit);
            console->log(line_bit);
            start = end;
        }
    }

    

}

int NewsGroup::status_code()
{
	return return_code;
}

// Private members go here.
// Protected members go here.
