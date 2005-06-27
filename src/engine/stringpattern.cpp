#include"news_constants.hpp"
#include"stringpattern.hpp"
#include"globals.hpp"

// Public data members go here.
StringPattern::StringPattern() 
{
    part_order.resize(SP_LASTPART);
}
    
StringPattern::~StringPattern() 
{
}

int StringPattern::does_match(string target)
{
    list<string>::iterator i;
    i = breakers.begin();
    while(i != breakers.end()){
        size_t index = target.find(*i);

        if(index != string::npos){
            target.erase(0, (*i).length() + index);
        }else{
            return 0;
        }

        ++i;
    }
    return 1;
}

void StringPattern::add_breaker(string target)
{
    breakers.push_back(target);
}

void StringPattern::add_breaker(SubjectPart target)
{
    part_order[target] = (SubjectPart)breakers.size();
}

void StringPattern::pieces(string target, vector<string>& result)
{
    list<string>::iterator i;
    i = breakers.begin();
    while(i != breakers.end()){
        size_t index = target.find(*i);

        if(index != string::npos){
            string flesh = target.substr(0, index);
            result.push_back(flesh);
            target.erase(0, (*i).length() + index);
        }else{
            return;
        }

        ++i;
    }
}

// Private members go here.
// Protected members go here.
